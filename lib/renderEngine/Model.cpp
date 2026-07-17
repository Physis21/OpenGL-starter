#include "Model.hpp"

void Model::Draw(Shader &shader){
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
	m_Meshes[i].Draw(shader);
}

void Model::loadModel(const std::string& path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene ->mRootNode) {
	std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
	return;
    }
    m_Directory = path.substr(0, path.find_last_of('/'));
    /* Because each node possibly contains a set of children we want to first process the root node,
     * so we use the recursive `processNode` function */
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // Process all the node's meshes, if any
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
	// Recursively iterate through relations to keep parent-child relationships
	aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
	m_Meshes.push_back(processMesh(mesh, scene));
    }
    // Then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
	processNode(node->mChildren[i], scene);
    }
 
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i<mesh->mNumVertices; i++) {
	Vertex vertex;
	// process vertex positions, normals and texture coordinates
	// ...
	vertices.push_back(vertex);
    }
    //process indices
    // ...
    // process material
    if (mesh->mMaterialIndex >= 0) {
	// ...
    }
    return Mesh(vertices, indices, textures);
}
