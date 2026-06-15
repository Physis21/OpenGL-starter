#include <cstdint>
#include <functional>
#include <unordered_map>
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float2.hpp"
#include "tiny_obj_loader.h"

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

// Use a hash on tinyobj::index_t to only extract unique vertex combinations
struct IndexHash {
    size_t operator()(const tinyobj::index_t& i) const;
};

// Equal operator for comparing indices
struct IndexEqual {
    bool operator()(const tinyobj::index_t& a, const tinyobj::index_t& b) const;
};


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
};

class ObjLoader {
    public:
        /* From OBJ file path, get a vertex buffer, index buffer, and vertex buffer layout
         */
        ObjLoader(const std::string& filepath = "res/objects/cube/cube.obj", const std::string& mtl_search_path = "./");
        ~ObjLoader();

        std::vector<Vertex> GetVertices() { return m_Vertices; };
        std::vector<uint32_t> GetIndices() const { return m_Indices; };

    private:
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        std::unordered_map<tinyobj::index_t, size_t, IndexHash, IndexEqual> m_UniqueVertices;
};
