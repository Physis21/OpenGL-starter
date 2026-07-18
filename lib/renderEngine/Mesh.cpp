#include "Mesh.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<std::shared_ptr<Texture>>& textures)
    : m_Vertices(vertices),
      m_Indices(indices),
      m_Textures(textures),
      m_VBO(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex)), // require size in bytes
      m_EBO(m_Indices.data(), m_Indices.size()) // require **count**
{
    // m_VAO setup still needs the layout, so build it here
    setupMesh();
    std::cout << "Loaded Mesh. Textures in m_Textures: \n";
    for (const std::shared_ptr<Texture>& texture : textures) {
        std::cout << "type: " << texture->GetType() << ", path: " << texture->GetFilePath() << "\n";
    }
    std::cout << std::endl;
}


void Mesh::setupMesh() {
    VertexBufferLayout layout;
    layout.Push<float>(3); // Coords
    layout.Push<float>(3); // Normals
    layout.Push<float>(2); // TexCoord
    m_VAO.AddBuffer(m_VBO, layout);
}

void Mesh::Draw(Shader &shader) 
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    shader.Bind();
    for(unsigned int i = 0; i < m_Textures.size(); i++)
    {
        // retrieve texture number (the N in diffuse_textureN)
	std::string number;
	std::string name = m_Textures[i]->GetType();
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        // shader.SetUniform1i("material." + name + number, i);
        shader.SetUniform1i(name + number, i);
        m_Textures[i]->Bind(i);
    }

    // draw mesh
    Renderer renderer;
    renderer.Draw(m_VAO, m_EBO, shader);
    m_VAO.Unbind();
}
