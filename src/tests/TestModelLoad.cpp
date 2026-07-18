#include "TestModelLoad.hpp"

namespace test {

    TestModelLoad::TestModelLoad(GLFWwindow* window) 
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


        /************ Model & its shader *****************/
        std::cout << "Loading Model...\n";
        m_Model = std::make_unique<Model>("res/models/backpack/backpack.obj");
        std::cout << "Model successfully loaded\n";
        std::cout << "Loading Shader...\n";
        m_Shader = std::make_unique<Shader>("res/shaders/TestModelLoad.shader");
        std::cout << "Shader successfully loaded" << std::endl;


        /*********** Make Camera ************/
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        /* Create Camera using current display port */
        m_Camera = std::make_unique<Camera>(display_w, display_h, glm::vec3(0.0f, 0.5, 2.0f));

        /***********************************
         * Unbind buffers and programs
         ***********************************/
    }
    TestModelLoad::~TestModelLoad() {
        glDisable(GL_DEPTH_TEST); // Disable Z buffer for other tests, as by default OpenGL doesn't enable it
    }

    void TestModelLoad::OnUpdate(float deltatime, GLFWwindow* window) {
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
    void TestModelLoad::OnRender() {
        GLfloat clearColor[4] = {0.5f, 0.5f, 0.0f, 0.0f};
        GLCall(glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // Clear with Z buffer!

        m_Camera->UpdateMatrix(45.0f, 0.1f, 100.0f);
        glm::vec3 camPos = m_Camera->GetPosition(); // Updated by Inputs()
        m_Camera->Matrix(*m_Shader, "u_CamMatrix");
        // Here the shader is bound inside the function.
        m_Model->Draw(*m_Shader);
        // Updates and exports the camera matrix to the Vertex Shader
    }
    void TestModelLoad::OnImGuiRender() {
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            glm::vec3 cameraOrientation = m_Camera->GetOrientation();
            glm::vec3 cameraPosition = m_Camera->GetPosition();
            ImGui::Begin("My GUI window!");
            ImGui::TextWrapped("Here we test the loading of a model from an .obj file with"
                    "assimp, the Open Asset Import Library. This library supports various 3D file formats.");

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
