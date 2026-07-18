#pragma once

#include <stdexcept>
#include <cstdint>
#include <functional>
#include <unordered_map>
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float2.hpp"
#include "tiny_obj_loader.h"

#include "VertexStruct.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "VertexArray.hpp"

// Use a hash on tinyobj::index_t to only extract unique vertex combinations
struct IndexHash {
    size_t operator()(const tinyobj::index_t& i) const;
};

// Equal operator for comparing indices
struct IndexEqual {
    bool operator()(const tinyobj::index_t& a, const tinyobj::index_t& b) const;
};


class ObjLoader {
    public:
        /* From OBJ file path, get a vertex buffer, index buffer, and vertex buffer layout
         * Throws runtime exception if there is no material, shape, normal indices, texcoord indices.
         * Also throws runtime exception on tinyobj errors.
         */
        ObjLoader(const std::string& filepath = "res/objects/cube/cube.obj", const std::string& mtl_search_path = "res/objects/cube");

        std::vector<Vertex> GetVertices() const { return m_Vertices; };
        std::vector<uint32_t> GetIndices() const { return m_Indices; };
        tinyobj::material_t GetMaterial() const { return m_Material; };

    private:
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        std::unordered_map<tinyobj::index_t, size_t, IndexHash, IndexEqual> m_UniqueVertices;
        tinyobj::material_t m_Material;
};

// Make vectors of vertices printable
std::ostream& operator<<(std::ostream& os, const std::vector<Vertex>& vv);
