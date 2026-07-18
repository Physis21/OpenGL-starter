#include "Mesh.hpp"

std::ostream& operator<<(std::ostream& os, const ModelTexture& texture) {
    os << "id: " << texture.id << ", type: " << texture.type << ", path: " << texture.path;
    return os;
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<ModelTexture>& textures)
    : m_Vertices(vertices),
      m_Indices(indices),
      m_Textures(textures),
      m_VBO(m_Vertices.data(), m_Vertices.size()),
      m_EBO(m_Indices.data(), m_Indices.size())
{
    // m_VAO setup still needs the layout, so build it here
    setupMesh();
    // std::cout << "Loaded Mesh. Textures in m_Textures: \n";
    // for (const ModelTexture& texture : textures) {
    //     std::cout << texture << "\n";
    // }
    // std::cout << std::endl;
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
	std::string name = m_Textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        // shader.SetUniform1i(("material." + name + number).c_str(), i);
        shader.SetUniform1i((name + number).c_str(), i);
        GLCall(glBindTexture(GL_TEXTURE_2D, m_Textures[i].id));
        GLCall(glActiveTexture(GL_TEXTURE0 + i)); // Different than learnOpengl.com, I think they made a mistake
    }

    // draw mesh
    Renderer renderer;
    renderer.Draw(m_VAO, m_EBO, shader);
    m_VAO.Unbind();
}
