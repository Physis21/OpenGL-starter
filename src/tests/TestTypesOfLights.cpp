#include "TestTypesOfLights.hpp"

namespace test {

    TestTypesOfLights::TestTypesOfLights(GLFWwindow* window) 
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


        /************ Flat Plane *****************/
        std::array<GLfloat, 44> vertices =
        { //     COORDINATES     /        COLORS        /    TexCoord    /       NORMALS     //
            -1.0f, 0.0f,  1.0f,     0.0f, 0.0f, 0.0f,	0.0f, 0.0f,	0.0f, 1.0f, 0.0f,
            -1.0f, 0.0f, -1.0f,     0.0f, 0.0f, 0.0f,	0.0f, 1.0f,	0.0f, 1.0f, 0.0f,
             1.0f, 0.0f, -1.0f,     0.0f, 0.0f, 0.0f,	1.0f, 1.0f,	0.0f, 1.0f, 0.0f,
             1.0f, 0.0f,  1.0f,     0.0f, 0.0f, 0.0f,	1.0f, 0.0f,	0.0f, 1.0f, 0.0f
        };

        // Indices for vertices order
        std::array<GLuint, 6> indices =
        {
            0, 1, 2,
            0, 2, 3,
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

        glm::vec3 plane2dPos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::mat4 plane2dModel = glm::mat4(1.0f);
        plane2dModel = glm::translate(plane2dModel, plane2dPos);

        m_Shader = std::make_unique<Shader>("res/shaders/TestTypesOfLights/Plane2d.shader");
        m_Shader->Bind();
        m_Texture0 = std::make_unique<Texture>("res/textures/planks.png", true, GL_RGBA);
        m_Texture1 = std::make_unique<Texture>("res/textures/planksSpec.png", true, GL_RED);
        m_Shader->SetUniform1i("u_Tex0", 0); // 0 because we bind the texture to slot 0
        m_Shader->SetUniform1i("u_Tex1", 1); // 1 because we bind the texture to slot 1
        m_Shader->SetUniformMat4f("u_Model", plane2dModel);

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
        glm::vec3 lightPos = glm::vec3(0.1f, 0.5f, 0.3f);
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);

        m_ShaderCube = std::make_unique<Shader>("res/shaders/TestTypesOfLights/Cube.shader");
        m_ShaderCube->Bind();
        m_ShaderCube->SetUniformMat4f("u_Model", lightModel);
        m_ShaderCube->SetUniform4f("u_LightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);

        // Add light color and pos to plane2d shader uniforms too
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
        m_Texture0->Unbind();
        m_Texture1->Unbind();

        m_VAOCube->Unbind();
        m_VertexBufferCube->Unbind();
        m_IndexBufferCube->Unbind();
        m_ShaderCube->Unbind();
    }
    TestTypesOfLights::~TestTypesOfLights() {
        glDisable(GL_DEPTH_TEST); // Disable Z buffer for other tests, as by default OpenGL doesn't enable it
    }

    void TestTypesOfLights::OnUpdate(float deltatime, GLFWwindow* window) {
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
    void TestTypesOfLights::OnRender() {
        Renderer renderer;
        GLfloat clearColor[4] = {0.5f, 0.5f, 0.0f, 0.0f};
        GLCall(glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // Clear with Z buffer!

        m_Camera->UpdateMatrix(45.0f, 0.1f, 100.0f);
        glm::vec3 camPos = m_Camera->GetPosition(); // Updated by Inputs()
        // Here we have redundant Shader bindings. In actual game engines, a caching system would be implemented to prevent this.
        {
            m_Shader->Bind();
            m_Texture0->Bind(0);
            m_Texture1->Bind(1);
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
    void TestTypesOfLights::OnImGuiRender() {
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            glm::vec3 cameraOrientation = m_Camera->GetOrientation();
            ImGui::Begin("My GUI window!");
            ImGui::TextWrapped("Here a second texture is added to the 2d Plane, with a single channel.\n"
                    "It is the specular map, and is used as the 'specular' variable in Phong shading.");
            ImGui::TextWrapped("Current Camera orientation: (%.2f , %.2f, %.2f)",
                    cameraOrientation[0],
                    cameraOrientation[1],
                    cameraOrientation[2]);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_ImGuiIO.Framerate, m_ImGuiIO.Framerate);
            ImGui::End();
        }
    }
}
