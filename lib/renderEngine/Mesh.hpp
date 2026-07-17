#pragma once

#include "VertexArray.hpp"
#include "VertexStruct.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include <vector>

class Mesh {
    private:
        std::vector<Vertex>         m_Vertices;
        std::vector<unsigned int>   m_Indices;
        std::vector<Texture>        m_Textures;

        VertexBuffer m_VBO;
        IndexBuffer m_EBO;
        VertexArray m_VAO;

        void setupMesh();

    public:
        // mesh data
        Mesh(
                const std::vector<Vertex>& vertices,
                const std::vector<unsigned int>& indices,
                const std::vector<Texture>& textures
        );
        std::vector<Vertex>         GetVertices() const;
        std::vector<unsigned int>   GetIndices() const;
        std::vector<Texture>        GetTextures() const;

        void Draw(Shader& shader);
};
