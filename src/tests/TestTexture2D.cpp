#include "TestTexture2D.hpp"

namespace test {

    TestTexture2D::TestTexture2D(GLFWwindow* window) 
    : 
        // glm::ortho simply drops the Z coordinate, and lines stay parallel. Objects look the same regardless of depth.
        m_Proj(glm::ortho(-320.0f, 320.0f, -240.0f, 240.0f, -240.0f, 240.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_TranslationA(0, 0, 0), 
        m_TranslationB(150, 0, 0),
        m_ShowAnotherWindow(false),
        m_ClearColor{0.0f, 0.0f, 0.0f, 1.00f},
        m_ImGuiIO(ImGui::GetIO())
    {
        (void) m_ImGuiIO;
        /* Square with 2 triangles 
         * 2 positions, then 2 texture coordinates */
        float positions[] = {
            -100, -140, 0.0f, 0.0f, // 0
             100, -140, 1.0f, 0.0f, // 1
             100,  140, 1.0f, 1.0f, // 2
            -100,  140, 0.0f, 1.0f, // 3
        };

        /* indices HAVE to be unsigned */
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
        };

        /* Blending 
         * TODO: move into Renderer init?*/
        GLCall(glEnable(GL_BLEND));
        /* Default: glBlendFunc(src=GL_ONE, dest=GL_ZERO) 
         * The source gets overriden by zeros in this case,
         * and the default glBlendEquation(GL_FUNC_ADD) simply adds src+dest 
         * NOTE: the alpha value also goes through the equation */
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();

        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        m_Shader = std::make_unique<Shader>("res/shaders/TestTexture2D.shader");
        m_Shader->Bind();

        m_Texture = std::make_unique<Texture>("res/textures/TsukihimeLogo.png");
        m_Shader->SetUniform1i("u_Texture", 0); // 0 because we bind the texture to slot 0

        /* Unbind buffers and program. Apparently this is uneeded now. TODO: read why */
        // m_VAO->Unbind();
        // m_VertexBuffer->Unbind();
        // m_IndexBuffer->Unbind();
        // m_Shader->Unbind();

    }
    TestTexture2D::~TestTexture2D() {

    }

    void TestTexture2D::OnUpdate(float deltatime, GLFWwindow* window) {

    }
    void TestTexture2D::OnRender() {
        Renderer renderer;
        renderer.Clear(m_ClearColor);

        m_Texture->Bind();

        // Here we have redundant Shader bindings. In actual game engines, a caching system would be implemented to prevent this.
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model; // In OpenGL, projection is proj * model * view due to column major
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model ; // In OpenGL, projection is proj * model * view due to column major
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

    }
    void TestTexture2D::OnImGuiRender() {
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static int counter = 0;

            ImGui::Begin("My GUI window!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Another Window", &m_ShowAnotherWindow);

            ImGui::SliderFloat3("translationA", &m_TranslationA.x, -200.0f, 200.0f); // Giving .x ref gives pointer to x, y & z
            ImGui::SliderFloat3("translationB", &m_TranslationB.x, -200.0f, 200.0f); // Giving .x ref gives pointer to x, y & z
            ImGui::ColorEdit3("clear color", m_ClearColor); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_ImGuiIO.Framerate, m_ImGuiIO.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (m_ShowAnotherWindow)
        {
            ImGui::Begin("Another Window", &m_ShowAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                m_ShowAnotherWindow = false;
            ImGui::End();
        }
    }
}
