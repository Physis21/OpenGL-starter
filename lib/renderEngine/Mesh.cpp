#include "Mesh.hpp"

Mesh::Mesh(
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        const std::vector<std::shared_ptr<Texture>>& textures,
        const aiColor3D& Ka,
        const aiColor3D& Kd,
        const aiColor3D& Ks,
        const aiColor3D& Ke
)
    : m_Vertices(vertices),
      m_Indices(indices),
      m_Textures(textures),
      m_VBO(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex)), // require size in bytes
      m_EBO(m_Indices.data(), m_Indices.size()), // require **count**
      m_Ka(Ka),
      m_Kd(Kd),
      m_Ks(Ks),
      m_Ke(Ke)
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

        shader.SetUniform1i(name + number, i);
        m_Textures[i]->Bind(i);
    }
    shader.SetUniform3f("mtl_Ka", m_Ka.r, m_Ka.g, m_Ka.b);
    shader.SetUniform3f("mtl_Kd", m_Kd.r, m_Kd.g, m_Kd.b);
    shader.SetUniform3f("mtl_Ks", m_Ks.r, m_Ks.g, m_Ks.b);
    shader.SetUniform3f("mtl_Ke", m_Ke.r, m_Ke.g, m_Ke.b);

    // draw mesh
    Renderer renderer;
    renderer.Draw(m_VAO, m_EBO, shader);
    m_VAO.Unbind();
}
