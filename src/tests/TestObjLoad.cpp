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

        std::cout << "Number of shapes: " << shapes.size() << std::endl; // In this test, use a single cube
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

        // Extract the index buffer from the mesh indices of the single shape
        std::vector<unsigned int> indices;
        indices.reserve(shapes[0].mesh.indices.size());
        for (const auto& idx : shapes[0].mesh.indices) {
            indices.push_back(static_cast<unsigned int>(idx.vertex_index));
        }
        std::cout << "Size of extracted index buffer: " << indices.size() << std::endl;
        PrintVector<unsigned int>(indices);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices.data(), indices.size());
        std::cout << "IndexBuffer constructed OK" << std::endl;

        /* Test out iteration */
        // Loop over faces(polygon). In the case of a cube, num_face_vertices.size() = 12 = 6 faces * 2 triangles per face
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[0].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[0].mesh.num_face_vertices[f]); // Always equal to 3 in our case since each face is a triangle
            std::cout << "f = " << f << ", fv = " << fv << "\n";

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                // index_t gives vertex_index, normal_index, texcoord_index
                tinyobj::index_t idx = shapes[0].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                }

                std::cout
                    << "idx: " << idx.vertex_index << ", " << idx.normal_index << ", " << idx.texcoord_index
                    << "\n";

                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
            }
            index_offset += fv;
            // per-face material
            // shapes[0].mesh.material_ids[f];
        }

        /* EndTest out iteration */

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
        GLfloat clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        renderer.Clear(clearColor);

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
