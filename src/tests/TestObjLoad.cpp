#include "TestObjLoad.hpp"

namespace test {

    TestObjLoad::TestObjLoad() 
    : 
        // glm::perspective divides X and Y by Z (perspective divide).
        m_Proj(glm::perspective(
            glm::radians(45.0f),    // Field of view (vertical angle)
            4.0f / 3.0f,        // Aspect ratio
            0.1f,                   // Near plane
            1000.0f                 // Far plane
        )),
        // m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_View(glm::lookAtRH(
            glm::vec3(0.0f, 0.0f, 0.0f), // eye position
            glm::vec3(0.0f, 0.0f, -1.0f), // Looking down the Z axis
            glm::vec3(0.0f, 1.0f, 0.0f) // up direction
        )), // In this particular case, `lookAt()` produces the identity matrix.
        m_TranslationA(0, 0, -10.0f), 
        m_TranslationB(150, 0, -10.0f),
        m_XYRotationA{0.0f, 0.0f},
        m_XYRotationB{0.0f, 0.0f},
        m_ImGuiIO(ImGui::GetIO())
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


        /***********************************
         * Loading obj files with tinyobj 
         ***********************************/
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = "./"; // Path to material files
        tinyobj::ObjReader reader;
        
        if (!reader.ParseFromFile("res/objects/cube/cube.obj", reader_config)) {
          if (!reader.Error().empty())
              std::cerr << "TinyObjReader: " << reader.Error();
          exit(1);
        }

        if (!reader.Warning().empty())
          std::cout << "TinyObjReader: " << reader.Warning();

        const tinyobj::attrib_t& attrib = reader.GetAttrib();
        const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();
        const std::vector<tinyobj::material_t>& materials = reader.GetMaterials();

        std::cout << "Number of shapes: " << shapes.size() << std::endl;
        if (!shapes.empty())
            std::cout << "Number of indices: " << shapes[0].mesh.indices.size() << std::endl;

        m_VAO = std::make_unique<VertexArray>();

        std::cout << "Size of extracted vertex buffer: " << attrib.vertices.size() << std::endl;
        PrintVector<tinyobj::real_t>(attrib.vertices);
        m_VertexBuffer = std::make_unique<VertexBuffer>(attrib.vertices.data(), attrib.vertices.size() * sizeof(tinyobj::real_t));
        VertexBufferLayout layout;
        layout.Push<tinyobj::real_t>(3);
        // layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        // Only a single shape
        std::vector<unsigned int> indices;
        indices.reserve(shapes[0].mesh.indices.size());
        for (const auto& idx : shapes[0].mesh.indices) {
            indices.push_back(static_cast<unsigned int>(idx.vertex_index));
        }
        std::cout << "Size of extracted index buffer: " << indices.size() << std::endl;
        PrintVector<unsigned int>(indices);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices.data(), indices.size());
        std::cout << "IndexBuffer constructed OK" << std::endl;

        m_Shader = std::make_unique<Shader>("res/shaders/TestObjLoad.shader");
        m_Shader->Bind();

        // m_Texture = std::make_unique<Texture>("res/textures/TsukihimeLogo.png");
        // m_Shader->SetUniform1i("u_Texture", 0); // 0 because we bind the texture to slot 0
        /***********************************
         * Unbind buffers and programs
         ***********************************/
        // m_VAO->Unbind();
        // m_VertexBuffer->Unbind();
        // m_IndexBuffer->Unbind();
        // m_Shader->Unbind();
    }
    TestObjLoad::~TestObjLoad() {

    }

    void TestObjLoad::OnUpdate(float deltatime) {

    }
    void TestObjLoad::OnRender() {
        Renderer renderer;

        // m_Texture->Bind();

        // Here we have redundant Shader bindings. In actual game engines, a caching system would be implemented to prevent this.
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            model = glm::rotate(model, glm::radians(m_XYRotationA.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(m_XYRotationA.y), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 mvp = m_Proj * m_View * model; // In OpenGL, projection is proj * model * view due to column major
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            model = glm::rotate(model, glm::radians(m_XYRotationB.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(m_XYRotationB.y), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 mvp = m_Proj * m_View * model; // In OpenGL, projection is proj * model * view due to column major
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

    }
    void TestObjLoad::OnImGuiRender() {
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static int counter = 0;

            ImGui::Begin("My GUI window!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Object A transformations");
            ImGui::SliderFloat2("translationA x&y", &m_TranslationA.x, -200.0f, 200.0f);
            ImGui::SliderFloat("translationA z", &m_TranslationA.z, 0.0f, -400.0f);
            ImGui::SliderFloat2("rotationA x&y axis", &m_XYRotationA.x, -90.0f, 90.0f);

            ImGui::Text("Object B transformations");
            ImGui::SliderFloat2("translationB x&y", &m_TranslationB.x, -200.0f, 200.0f);
            ImGui::SliderFloat("translationB z", &m_TranslationB.z, 0.0f, -400.0f);
            ImGui::SliderFloat2("rotationB x&y axis", &m_XYRotationB.x, -90.0f, 90.0f);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_ImGuiIO.Framerate, m_ImGuiIO.Framerate);
            ImGui::End();
        }
    }
}
