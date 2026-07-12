#include "TestCamera.hpp"

namespace test {

    TestCamera::TestCamera(GLFWwindow* window) 
    : 
        m_ImGuiIO(ImGui::GetIO())
        // glm::perspective divides X and Y by Z (perspective divide).
    {
        /* Blending 
         * NOTE: move into Renderer init? I don't think that's the Renderer's role */
        GLCall(glEnable(GL_BLEND));
        /* Default: glBlendFunc(src=GL_ONE, dest=GL_ZERO) 
         * The source gets overriden by zeros in this case,
         * and the default glBlendEquation(GL_FUNC_ADD) simply adds src+dest 
         * NOTE: the alpha value also goes through the equation */
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        glEnable(GL_DEPTH_TEST); // Enable Z-buffer!

        /* Test out iteration over the single shape. */
        // Layout, VB, VAO init
        VertexBufferLayout layout;
        layout.Push<tinyobj::real_t>(3); // Coords
        layout.Push<tinyobj::real_t>(3); // Color
        layout.Push<tinyobj::real_t>(2); // TexCoord
        m_VAO = std::make_unique<VertexArray>();

        std::array<GLfloat, 40> vertices =
        { //     COORDINATES     /        COLORS      /   TexCoord  //
            -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
            -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
             0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
             0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
             0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
        };

        // Indices for vertices order
        std::array<GLuint, 18> indices =
        {
            0, 1, 2,
            0, 2, 3,
            0, 1, 4,
            1, 2, 4,
            2, 3, 4,
            3, 0, 4
        };
        m_VertexBuffer = std::make_unique<VertexBuffer>(vertices.data(), sizeof(vertices));
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices.data(), indices.size());

        /* EndTest out iteration */

        m_Shader = std::make_unique<Shader>("res/shaders/TestCamera.shader");
        m_Shader->Bind();

        m_Texture = std::make_unique<Texture>("res/textures/brick.png", true);
        m_Shader->SetUniform1i("u_Texture", 0); // 0 because we bind the texture to slot 0

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        /* Create Camera using current display port */
        m_Camera = std::make_unique<Camera>(display_w, display_h, glm::vec3(0.0f, 0.0f, 0.2f));

        /***********************************
         * Unbind buffers and programs
         ***********************************/
        m_VAO->Unbind();
        m_VertexBuffer->Unbind();
        m_IndexBuffer->Unbind();
        m_Shader->Unbind();
        m_Texture->Unbind();
    }
    TestCamera::~TestCamera() {
        glDisable(GL_DEPTH_TEST); // Disable Z buffer for other tests, as by default OpenGL doesn't enable it
    }

    void TestCamera::OnUpdate(float deltatime, GLFWwindow* window) {
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        /* Create Camera using current display port */
        m_Camera->SetHeight(display_w);
        m_Camera->SetHeight(display_h);
        // Handles camera inputs
        m_Camera->Inputs(window);
    }
    void TestCamera::OnRender() {
        Renderer renderer;
        GLfloat clearColor[4] = {0.1f, 0.0f, 0.0f, 0.0f};
        GLCall(glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // Clear with Z buffer!


        // Here we have redundant Shader bindings. In actual game engines, a caching system would be implemented to prevent this.
        {
            m_Shader->Bind();
            m_Texture->Bind();
            // Updates and exports the camera matrix to the Vertex Shader
            m_Camera->Matrix(45.0f, 0.1f, 100.0f, *m_Shader, "u_CamMatrix");

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

    }
    void TestCamera::OnImGuiRender() {
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("My GUI window!");
            ImGui::Text("Here user input is handled by GLFW in the `Camera` class.");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_ImGuiIO.Framerate, m_ImGuiIO.Framerate);
            ImGui::End();
        }
    }
}
