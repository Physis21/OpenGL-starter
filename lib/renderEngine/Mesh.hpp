#pragma once
#include "glad.h"

#include "GLErrorManager.hpp"
#include "VertexArray.hpp"
#include "VertexStruct.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include <memory>
#include <vector>

class Mesh {
    private:
        std::vector<Vertex>         m_Vertices;
        std::vector<unsigned int>   m_Indices;
        std::vector<std::shared_ptr<Texture>>  m_Textures;

        VertexBuffer m_VBO;
        IndexBuffer m_EBO;
        VertexArray m_VAO;

        void setupMesh();

    public:
        // mesh data
        Mesh(
                const std::vector<Vertex>& vertices,
                const std::vector<unsigned int>& indices,
                const std::vector<std::shared_ptr<Texture>>& textures
        );

        // delete copy constructors as Mesh owns 3 RAII wrappers around raw GL objects.
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;
        Mesh(Mesh&&) noexcept = default;      // fine as long as VBO/EBO/VAO/textures are all move-friendly
        Mesh& operator=(Mesh&&) noexcept = default;
        // Mesh(const Mesh&) = delete;
        // Mesh& operator=(const Mesh&) = delete;
        // Mesh(Mesh&&) noexcept = default;      // fine as long as VBO/EBO/VAO/textures are all move-friendly
        // Mesh& operator=(Mesh&&) noexcept = default;

        const std::vector<Vertex>&         GetVertices() const { return m_Vertices; };
        const std::vector<unsigned int>&   GetIndices() const { return m_Indices; };
        const std::vector<std::shared_ptr<Texture>>&  GetTextures() const { return m_Textures; };

        void Draw(Shader& shader);
};
