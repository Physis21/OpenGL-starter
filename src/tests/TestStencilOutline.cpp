#include "TestStencilOutline.hpp"

namespace test {

    TestStencilOutline::TestStencilOutline(GLFWwindow* window) 
    : 
        m_ImGuiIO(ImGui::GetIO())
        // glm::perspective divides X and Y by Z (perspective divide).
    {
        /* NOTE: In this test, blending is disabled.
        // GLCall(glEnable(GL_BLEND));
         * Default: glBlendFunc(src=GL_ONE, dest=GL_ZERO) 
         * The source gets overriden by zeros in this case,
         * and the default glBlendEquation(GL_FUNC_ADD) simply adds src+dest 
         * NOTE: the alpha value also goes through the equation */
        // GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        glEnable(GL_DEPTH_TEST); // Enable Z-buffer!
        GLCall(glEnable(GL_STENCIL_TEST));
        GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));


        /************ Model & its shader *****************/
        std::cout << "Loading Model...\n";
        m_Model = std::make_unique<Model>("res/models/bluecar_gltf/scene.gltf");
        std::cout << "Model successfully loaded\n";
        std::cout << "Loading Shaders...\n";
        m_Shader = std::make_unique<Shader>("res/shaders/TestStencilOutline/car.shader");
        m_NormalOutlineShader = std::make_unique<Shader>("res/shaders/TestStencilOutline/carNormalOutline.shader");
        m_SobelShader = std::make_unique<Shader>("res/shaders/TestStencilOutline/carSobel.shader");
        m_OutlineShader = std::make_unique<Shader>("res/shaders/TestStencilOutline/outline.shader");
        std::cout << "Shaders successfully loaded" << std::endl;


        /*********** Make Camera ************/
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        /* Create Camera using current display port */
        m_Camera = std::make_unique<Camera>(display_w, display_h, glm::vec3(0.0f, 0.5, 2.0f));

        /***********************************
         * Unbind buffers and programs
         ***********************************/
    }
    TestStencilOutline::~TestStencilOutline() {
        glDisable(GL_DEPTH_TEST); // Disable Z buffer for other tests, as by default OpenGL doesn't enable it
        glDisable(GL_STENCIL_TEST);
    }

    void TestStencilOutline::OnUpdate(float deltatime, GLFWwindow* window) {
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        /* Create Camera using current display port */
        m_Camera->SetWidth(display_w);
        m_Camera->SetHeight(display_h);
        // Handles camera inputs
        
        if (!m_ImGuiIO.WantCaptureMouse) {
            m_Camera->Inputs(window, deltatime);
        }
    }
    void TestStencilOutline::OnRender() {

        GLfloat clearColor[4] = {0.1f, 0.1f, 0.0f, 0.0f};
        GLCall(glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)); // Clear with Z buffer and Stencil buffer!
        
        m_Camera->UpdateMatrix(45.0f, 0.1f, 100.0f);

        if (m_ShaderChoice == StencilOutlineShaderChoice) {
            m_OutlineShader->Bind();
            m_Camera->Matrix(*m_OutlineShader, "u_CamMatrix");

            GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF)); // Pass test when the buffer val != 1 and depth test passes.
            GLCall(glStencilMask(0xFF)); // Disable writing to stencil mask to keep original silhouette.
            GLCall(glDisable(GL_DEPTH_TEST));
            m_OutlineShader->Bind();
            m_OutlineShader->SetUniform1f("u_Outlining", 1.08);
            m_Model->Draw(*m_OutlineShader);
            GLCall(glStencilMask(0x00));
            GLCall(glStencilFunc(GL_ALWAYS, 0, 0xFF));
            GLCall(glEnable(GL_DEPTH_TEST));
        }

        GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF)); // make sure we update the stencil buffer
        GLCall(glStencilMask(0xFF)); // make sure we update the stencil buffer
        if (m_ShaderChoice == RegularShaderChoice) {
            m_Shader->Bind();
            m_Camera->Matrix(*m_Shader, "u_CamMatrix");
            m_Model->Draw(*m_Shader);
        } else if (m_ShaderChoice == SobelShaderChoice){
            m_Shader->Bind();
            m_Camera->Matrix(*m_Shader, "u_CamMatrix");
            m_Model->Draw(*m_Shader);
        } else if (m_ShaderChoice == NormalOutlineShaderChoice){
            glm::vec3 camPos = m_Camera->GetPosition();
            m_NormalOutlineShader->Bind();
            m_Camera->Matrix(*m_NormalOutlineShader, "u_CamMatrix");
            m_NormalOutlineShader->SetUniform3f("u_CamPos", camPos.x, camPos.y, camPos.z);
            m_Model->Draw(*m_NormalOutlineShader);
        }

    }
    void TestStencilOutline::OnImGuiRender() {
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            glm::vec3 cameraOrientation = m_Camera->GetOrientation();
            glm::vec3 cameraPosition = m_Camera->GetPosition();
            ImGui::Begin("My GUI window!");
            ImGui::TextWrapped("Here we test the loading of a model from an .gltf file with"
                    "assimp, and the outlining of objects with the stencil buffer.");
            //
            // Popup to select shader
            const char* names[] = {
                "Regular",
                "Stencil outline with upscale",
                "Outline from normals",
                "Sobel shader (not finished)"
            };
            if (ImGui::Button("Select Shader"))
                ImGui::OpenPopup("shader_choice");
            ImGui::SameLine();
            ImGui::TextUnformatted(names[m_ShaderChoice]);
            if (ImGui::BeginPopup("shader_choice"))
            {
                ImGui::SeparatorText("Outlining Shader program");
                for (int i = RegularShaderChoice; i != Last; i++)
                    if (ImGui::Selectable(names[i]))
                        m_ShaderChoice = static_cast<ShaderChoice>(i);
                ImGui::EndPopup();
            }

            // Misc metrics
            ImGui::TextWrapped("Current Camera orientation: (%.2f , %.2f, %.2f)",
                    cameraOrientation[0],
                    cameraOrientation[1],
                    cameraOrientation[2]);
            ImGui::TextWrapped("Current Camera position: (%.2f , %.2f, %.2f)",
                    cameraPosition[0],
                    cameraPosition[1],
                    cameraPosition[2]);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_ImGuiIO.Framerate, m_ImGuiIO.Framerate);
            ImGui::End();
        }
    }
}
