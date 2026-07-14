#include "TestLighting.hpp"

namespace test {

    TestLighting::TestLighting(GLFWwindow* window) 
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


        /************ Pyramid *****************/
        std::array<GLfloat, 176> vertices =
            { //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
            -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
            -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
             0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
             0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

            -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
            -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
             0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

            -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
             0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
             0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

             0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
             0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
             0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

             0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
            -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
             0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
        };

        // Indices for vertices order
        std::array<GLuint, 18> indices =
        {
            0, 1, 2, // Bottom side
            0, 2, 3, // Bottom side
            4, 6, 5, // Left side
            7, 9, 8, // Non-facing side
            10, 12, 11, // Right side
            13, 15, 14 // Facing side
        };
        // Layout, VB, VAO init
        VertexBufferLayout layout;
        layout.Push<float>(3); // Coords
        layout.Push<float>(3); // Color
        layout.Push<float>(2); // TexCoord
        layout.Push<float>(3); // Normals
        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(vertices.data(), sizeof(vertices));
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices.data(), indices.size());

        glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::mat4 pyramidModel = glm::mat4(1.0f);
        pyramidModel = glm::translate(pyramidModel, pyramidPos);

        m_Shader = std::make_unique<Shader>("res/shaders/TestLighting/Pyramid.shader");
        m_Shader->Bind();
        m_Texture = std::make_unique<Texture>("res/textures/brick.png", true);
        m_Shader->SetUniform1i("u_Tex0", 0); // 0 because we bind the texture to slot 0
        m_Shader->SetUniformMat4f("u_Model", pyramidModel);

        /************ Light cube *****************/
        std::array<GLfloat, 24> lightVertices =
        { //     COORDINATES     //
            -0.1f, -0.1f,  0.1f,
            -0.1f, -0.1f, -0.1f,
             0.1f, -0.1f, -0.1f,
             0.1f, -0.1f,  0.1f,
            -0.1f,  0.1f,  0.1f,
            -0.1f,  0.1f, -0.1f,
             0.1f,  0.1f, -0.1f,
             0.1f,  0.1f,  0.1f
        };
        std::array<GLuint, 36> lightIndices = 
        {
            0, 1, 2,
            0, 2, 3,
            0, 4, 7,
            0, 7, 3,
            3, 7, 6,
            3, 6, 2,
            2, 6, 5,
            2, 5, 1,
            1, 5, 4,
            1, 4, 0,
            4, 5, 6,
            4, 6, 7
        };
        // Layout, VB, VAO init
        VertexBufferLayout layoutCube;
        layoutCube.Push<float>(3); // Coords
        m_VAOCube = std::make_unique<VertexArray>();
        m_VertexBufferCube = std::make_unique<VertexBuffer>(lightVertices.data(), sizeof(lightVertices));
        m_VAOCube->AddBuffer(*m_VertexBufferCube, layoutCube);
        m_IndexBufferCube = std::make_unique<IndexBuffer>(lightIndices.data(), lightIndices.size());

        glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);

        m_ShaderCube = std::make_unique<Shader>("res/shaders/TestLighting/Cube.shader");
        m_ShaderCube->Bind();
        m_ShaderCube->SetUniformMat4f("u_Model", lightModel);
        m_ShaderCube->SetUniform4f("u_LightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);

        // Add light color and pos to pyramid shader uniforms too
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_LightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        m_Shader->SetUniform3f("u_LightPos", lightPos.x, lightPos.y, lightPos.z);

        /*********** Make Camera ************/
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        /* Create Camera using current display port */
        m_Camera = std::make_unique<Camera>(display_w, display_h, glm::vec3(0.0f, 0.5, 2.0f));

        /***********************************
         * Unbind buffers and programs
         ***********************************/
        m_VAO->Unbind();
        m_VertexBuffer->Unbind();
        m_IndexBuffer->Unbind();
        m_Shader->Unbind();
        m_Texture->Unbind();

        m_VAOCube->Unbind();
        m_VertexBufferCube->Unbind();
        m_IndexBufferCube->Unbind();
        m_ShaderCube->Unbind();
    }
    TestLighting::~TestLighting() {
        glDisable(GL_DEPTH_TEST); // Disable Z buffer for other tests, as by default OpenGL doesn't enable it
    }

    void TestLighting::OnUpdate(float deltatime, GLFWwindow* window) {
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        /* Create Camera using current display port */
        m_Camera->SetHeight(display_w);
        m_Camera->SetHeight(display_h);
        // Handles camera inputs
        
        if (!m_ImGuiIO.WantCaptureMouse) {
            m_Camera->Inputs(window);
        }
    }
    void TestLighting::OnRender() {
        Renderer renderer;
        GLfloat clearColor[4] = {0.1f, 0.0f, 0.0f, 0.0f};
        GLCall(glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // Clear with Z buffer!

        m_Camera->UpdateMatrix(45.0f, 0.1f, 100.0f);
        glm::vec3 camPos = m_Camera->GetPosition(); // Updated by Inputs()
        // Here we have redundant Shader bindings. In actual game engines, a caching system would be implemented to prevent this.
        {
            m_Shader->Bind();
            m_Texture->Bind();
            // Updates and exports the camera matrix to the Vertex Shader
            m_Camera->Matrix(*m_Shader, "u_CamMatrix");
            m_Shader->SetUniform3f("u_CamPos", camPos.x, camPos.y, camPos.z);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
        {
            m_ShaderCube->Bind();
            // Updates and exports the camera matrix to the Vertex Shader
            m_Camera->Matrix(*m_ShaderCube, "u_CamMatrix");
            renderer.Draw(*m_VAOCube, *m_IndexBufferCube, *m_ShaderCube);
        }

    }
    void TestLighting::OnImGuiRender() {
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            glm::vec3 cameraOrientation = m_Camera->GetOrientation();
            ImGui::Begin("My GUI window!");
            ImGui::TextWrapped("Here user input is handled by GLFW in the `Camera` class.\n"
                    "Currently, all camera inputs are disabled when hovering ImGUI windows.");
            ImGui::TextWrapped("Current Camera orientation: (%.2f , %.2f, %.2f)",
                    cameraOrientation[0],
                    cameraOrientation[1],
                    cameraOrientation[2]);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_ImGuiIO.Framerate, m_ImGuiIO.Framerate);
            ImGui::End();
        }
    }
}
