/*
 * When loading OBJ files with tinyobj, it is more standard to preprocess to data so that
 * all the OBJ data is stored in a single Vertex Buffer Object.
 *
 * The reason is that using multiple VBOs induces more state changes, and is less GPU cache friendly.
 * Multiple VBOs would be good if attributes updated at different frequencies,
 * you want to re-use an attribute buffer across multiple meshes, or you're doing GPU-driven rendering
 * with attribute streams that vary at every pass.
 *
 * In the Vertex Buffer, I store: pos.x, pos.y, pos.z, norm.x, norm.y, norm.z tex.u, tex.v
 *
 * Scheme for indexing: i_tex + i_pos * 
 */

#include "TestObjLoad.hpp"

namespace test {

    TestObjLoad::TestObjLoad() 
    : 
        // glm::perspective divides X and Y by Z (perspective divide).
        m_Proj(glm::perspective(
            glm::radians(45.0f),    // Field of view (vertical angle)
            4.0f / 3.0f,        // Aspect ratio
            0.1f,                   // Near plane
            20.0f                 // Far plane
        )),
        // m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_View(glm::lookAtRH(
            glm::vec3(0.0f, 0.0f, 0.0f), // eye position
            glm::vec3(0.0f, 0.0f, -1.0f), // Looking down the Z axis
            glm::vec3(0.0f, 1.0f, 0.0f) // up direction
        )), // In this particular case, `lookAt()` produces the identity matrix.
        m_TranslationA(0, 0, -5.0f), 
        m_TranslationB(2, 0, -5.0f),
        m_XYRotationA{20.0f, 20.0f},
        m_XYRotationB{20.0f, 20.0f},
        m_ImGuiIO(ImGui::GetIO()),
        m_LightDirection(glm::vec4(-0.2f, -1.0f, 0.0f, 1.0f))
    {
        (void) m_ImGuiIO; // Compiler unused warning suppresion trick

        /* Blending 
         * NOTE: move into Renderer init? I don't think that's the Renderer's role */
        GLCall(glEnable(GL_BLEND));
        /* Default: glBlendFunc(src=GL_ONE, dest=GL_ZERO) 
         * The source gets overriden by zeros in this case,
         * and the default glBlendEquation(GL_FUNC_ADD) simply adds src+dest 
         * NOTE: the alpha value also goes through the equation */
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /* Test out iteration over the single shape. */
        // Layout, VB, VAO init
        VertexBufferLayout layout;
        layout.Push<tinyobj::real_t>(3); // Vertex index
        layout.Push<tinyobj::real_t>(3); // Normal index
        layout.Push<tinyobj::real_t>(2); // TexCoord index
        m_VAO = std::make_unique<VertexArray>();

        // Get vertex and index buffer from objLoader
        std::string mtl_path = "res/objects/cube/";
        ObjLoader objLoader("res/objects/cube/cube.obj", mtl_path);
        std::vector<Vertex> vertices = objLoader.GetVertices();
        std::vector<uint32_t> indices = objLoader.GetIndices();
        tinyobj::material_t material = objLoader.GetMaterial();

        std::cout << "Vertex buffer: \n";
        std::cout << vertices;
        std::cout << "index buffer: \n";
        for (int i = 0; i < indices.size(); i++) {
            std::cout << indices[i] << " ";
        }
        std::cout << std::endl;

        m_VertexBuffer = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(Vertex));
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices.data(), indices.size());

        /* EndTest out iteration */

        m_Shader = std::make_unique<Shader>("res/shaders/ObjWithText.shader");
        m_Shader->Bind();

        std::cout << "texture to load: " << mtl_path + material.diffuse_texname << "\n";
        m_Texture = std::make_unique<Texture>(mtl_path + material.diffuse_texname, true);
        m_Shader->SetUniform1i("u_Texture", 0); // 0 because we bind the texture to slot 0
        /***********************************
         * Unbind buffers and programs
         ***********************************/
        m_VAO->Unbind();
        m_VertexBuffer->Unbind();
        m_IndexBuffer->Unbind();
        m_Shader->Unbind();

        glEnable(GL_DEPTH_TEST); // Enable Z-buffer!
    }
    TestObjLoad::~TestObjLoad() {
        glDisable(GL_DEPTH_TEST); // Disable Z buffer for other tests
    }

    void TestObjLoad::OnUpdate(float deltatime) {

    }
    void TestObjLoad::OnRender() {
        Renderer renderer;
        GLfloat clearColor[4] = {0.1f, 0.0f, 0.0f, 0.0f};
        GLCall(glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // Clear with Z buffer!

        m_Texture->Bind();

        // Here we have redundant Shader bindings. In actual game engines, a caching system would be implemented to prevent this.
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            model = glm::rotate(model, glm::radians(m_XYRotationA.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(m_XYRotationA.y), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 mvp = m_Proj * m_View * model; // In OpenGL, projection is proj * model * view due to column major
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            m_Shader->SetUniformMat4f("u_Model", model);
            m_Shader->SetUniform4f("u_LightDirection", m_LightDirection[0], m_LightDirection[1], m_LightDirection[2], m_LightDirection[3]);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            model = glm::rotate(model, glm::radians(m_XYRotationB.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(m_XYRotationB.y), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 mvp = m_Proj * m_View * model; // In OpenGL, projection is proj * model * view due to column major
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            m_Shader->SetUniformMat4f("u_Model", model);
            m_Shader->SetUniform4f("u_LightDirection", m_LightDirection[0], m_LightDirection[1], m_LightDirection[2], m_LightDirection[3]);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

    }
    void TestObjLoad::OnImGuiRender() {
        float maxXOrY = 10.0f;
        float maxZ = 20.0f;
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static int counter = 0;

            ImGui::Begin("My GUI window!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Object A transformations");
            ImGui::SliderFloat2("translationA x&y", &m_TranslationA.x, -maxXOrY, maxXOrY);
            ImGui::SliderFloat("translationA z", &m_TranslationA.z, 0.0f, -maxZ);
            ImGui::SliderFloat2("rotationA x&y axis", &m_XYRotationA.x, -90.0f, 90.0f);

            ImGui::Text("Object B transformations");
            ImGui::SliderFloat2("translationB x&y", &m_TranslationB.x, -maxXOrY, maxXOrY);
            ImGui::SliderFloat("translationB z", &m_TranslationB.z, 0.0f, -maxZ);
            ImGui::SliderFloat2("rotationB x&y axis", &m_XYRotationB.x, -90.0f, 90.0f);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_ImGuiIO.Framerate, m_ImGuiIO.Framerate);
            ImGui::End();
        }
    }
}
