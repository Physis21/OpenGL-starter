#include "TestTexture3D.hpp"
#include <array>

namespace test {

    TestTexture3D::TestTexture3D() 
    : 
        m_Proj(glm::ortho(-320.0f, 320.0f, -240.0f, 240.0f, 0.0f, 480.0f)),
        // m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_View(glm::lookAtRH(
            glm::vec3(0.0f, 0.0f, 0.0f), // eye position
            glm::vec3(0.0f, 0.0f, -1.0f), // Looking down the Z axis
            glm::vec3(0.0f, 1.0f, 0.0f) // up direction
        )), // In this particular case, `lookAt()` produces the identity matrix.
        m_TranslationA(0, 0, 0), 
        m_TranslationB(150, 0, 0),
        m_ClearColor{0.0f, 0.0f, 0.0f, 1.00f},
        m_ImGuiIO(ImGui::GetIO())
    {
        (void) m_ImGuiIO; // Compiler unused warning suppresion trick
        /* Square with 2 triangles, possibly oriented.
         * 3 positions, then 2 texture coordinates */
        std::array<float, 20> positions = {
            -100, -140, -100.0f ,0.0f, 0.0f, // 0
             100, -140, -100.0f, 1.0f, 0.0f, // 1
             100,  140, -100.0f, 1.0f, 1.0f, // 2
            -100,  140, -100.0f, 0.0f, 1.0f, // 3
        };

        /* indices HAVE to be unsigned */
        std::array<unsigned int, 6> indices = {
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

        m_VertexBuffer = std::make_unique<VertexBuffer>(positions.data(), positions.size() * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices.data(), indices.size());

        m_Shader = std::make_unique<Shader>("res/shaders/TestTexture3D.shader");
        m_Shader->Bind();

        m_Texture = std::make_unique<Texture>("res/textures/TsukihimeLogo.png");
        m_Shader->SetUniform1i("u_Texture", 0); // 0 because we bind the texture to slot 0

        /* Unbind buffers and program. Apparently this is uneeded now. TODO: read why */
        // m_VAO->Unbind();
        // m_VertexBuffer->Unbind();
        // m_IndexBuffer->Unbind();
        // m_Shader->Unbind();

    }
    TestTexture3D::~TestTexture3D() {

    }

    void TestTexture3D::OnUpdate(float deltatime) {

    }
    void TestTexture3D::OnRender() {
        GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
        // GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind();

        // Here we have redundant Shader bindings. In actual game engines, a caching system would be implemented to prevent this.
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * model * m_View; // In OpenGL, projection is proj * model * view due to column major
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * model * m_View; // In OpenGL, projection is proj * model * view due to column major
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

    }
    void TestTexture3D::OnImGuiRender() {
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static int counter = 0;

            ImGui::Begin("My GUI window!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

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
    }
}
