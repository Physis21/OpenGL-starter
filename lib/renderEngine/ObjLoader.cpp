#include "ObjLoader.hpp"

size_t IndexHash::operator()(const tinyobj::index_t& i) const {
    size_t h = 0;
    auto combine = [&](size_t val) {
        h ^= val + 0x9e3779b9 + (h << 6) + (h >> 2);
    };
    combine(std::hash<int>{}(i.vertex_index));
    combine(std::hash<int>{}(i.normal_index));
    combine(std::hash<int>{}(i.texcoord_index));
    return h;
}

bool IndexEqual::operator()(const tinyobj::index_t& a, const tinyobj::index_t& b) const {
    return a.vertex_index   == b.vertex_index
        && a.normal_index   == b.normal_index
        && a.texcoord_index == b.texcoord_index;
}

ObjLoader::ObjLoader(const std::string& filepath, const std::string& mtl_search_path) {
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = mtl_search_path; // Path to material files
    tinyobj::ObjReader reader;
    
    // Find file and parse it
    if (!reader.ParseFromFile(filepath, reader_config)) {
      if (!reader.Error().empty())
          std::cerr << "TinyObjReader: " << reader.Error();
      exit(1);
    }

    // If there is a warning from the parser, display it
    if (!reader.Warning().empty())
      std::cout << "TinyObjReader: " << reader.Warning();

    const tinyobj::attrib_t& attrib = reader.GetAttrib();
    const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();
    const std::vector<tinyobj::material_t>& materials = reader.GetMaterials();

    // Log info about the loaded shape
    std::cout << "Number of shapes: " << shapes.size() << std::endl; // In this test, use a single cube
    if (!shapes.empty())
        std::cout << "Number of indices: " << shapes[0].mesh.indices.size() << std::endl;

    // Initialize empty vectors implicitly
    m_Vertices = {};
    m_Indices = {};

    // At the moment, only support a single shape per OBJ file
    const auto shape = shapes[0];
    for (const tinyobj::index_t& idx : shape.mesh.indices) {
        Vertex vertex{};

        vertex.position = {
            attrib.vertices[3 * idx.vertex_index + 0],
            attrib.vertices[3 * idx.vertex_index + 1],
            attrib.vertices[3 * idx.vertex_index + 2],
        };
        vertex.normal = {
            attrib.normals[3 * idx.normal_index + 0],
            attrib.normals[3 * idx.normal_index + 1],
            attrib.normals[3 * idx.normal_index + 2],
        };
        vertex.texcoord = {
            attrib.texcoords[2 * idx.texcoord_index + 0],
            attrib.texcoords[2 * idx.texcoord_index + 1],
        };

        // Only add to VBO if this exact combination hasn't been seen
        // TODO: adapt from Claude code to hashing Indices
        if (m_UniqueVertices.count(idx) == 0) {
            m_UniqueVertices[idx] = m_Vertices.size();
            m_Vertices.push_back(vertex);
        }

        m_Indices.push_back(m_UniqueVertices[idx]);
    }

    // TODO: Create VertexBufferLayout, and create VAO. Then assign values to the VertexBufferObject and IndexBuffer to return with getters
}
ObjLoader::~ObjLoader() {

}

