#include "TestStencilOutline.hpp"

namespace test {

    TestStencilOutline::TestStencilOutline(GLFWwindow* window) 
    : 
        m_ImGuiIO(ImGui::GetIO())
        // glm::perspective divides X and Y by Z (perspective divide).
    {
        GLCall(glEnable(GL_DEPTH_TEST)); // Enable Z-buffer!
        GLCall(glEnable(GL_STENCIL_TEST));

        /* Decides what happends after the test.
         * in cases :
         * - sfail & dpfail: we choose to keep the stored ref value
         * - dppass: we replace the stencil ref value
         * So if the fragment is actually drawn, write something into the stencil buffer, else leave it alone.
         */
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

        /* 0xFF read mask means all bits are used in ref and current pixel stencil value
         * Here, we make sure we update the stencil test always passes */
        GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
        /* glStencilMask controls which bit in the stencil buffer any write can make
         * Here, we make sure we update the stencil buffer */
        GLCall(glStencilMask(0xFF));

        if (m_ShaderChoice == StencilOutlineShaderChoice) {
            // From https://www.youtube.com/watch?v=ngF9LWWxhd0&list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-&index=16
            // Not a conventional way of outlining, but somewhat works. Pretty complicated, from my point of view.
            
            // 1st pass: draw the car normally. Every fragment writes 1 into the stencil buffer,
            // which marks the car's silhouette.
            m_Shader->Bind();
            m_Camera->Matrix(*m_Shader, "u_CamMatrix");
            m_Model->Draw(*m_Shader);

            // 2nd pass: draw the upscaled car, only where the stencil buffer != 1 (outside the silhouette).
            GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF)); // Inside the silhouette, this comparison will fail
            GLCall(glStencilMask(0x00)); // Disable writing to the stencil buffer to keep the original silhouette.
            GLCall(glDisable(GL_DEPTH_TEST)); // Outline is drawn on top of everything

            m_OutlineShader->Bind();
            m_Camera->Matrix(*m_OutlineShader, "u_CamMatrix");
            m_OutlineShader->SetUniform1f("u_Outlining", 1.08);
            m_Model->Draw(*m_OutlineShader);

            // Restore state. The stencil mask must be 0xFF again, otherwise
            // glClear(GL_STENCIL_BUFFER_BIT) won't clear the stencil buffer next frame.
            GLCall(glStencilMask(0xFF));
            GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
            GLCall(glEnable(GL_DEPTH_TEST));
        } else if (m_ShaderChoice == RegularShaderChoice) {
            m_Shader->Bind();
            m_Camera->Matrix(*m_Shader, "u_CamMatrix");
            m_Model->Draw(*m_Shader);
        } else if (m_ShaderChoice == SobelShaderChoice){
            // Not implemented yet, but reportedly a more standard way of making outlines.
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
