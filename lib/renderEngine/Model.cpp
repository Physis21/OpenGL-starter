#include "Model.hpp"

Model::Model(const std::string& path, bool gamma): m_GammaCorrection(gamma)  {
    loadModel(path);
}

void Model::Draw(Shader &shader){
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
	m_Meshes[i].Draw(shader);
}

void Model::loadModel(const std::string& path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

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
    std::cout << "In node " << node->mName.C_Str() << ", there are " << node->mNumMeshes << " meshes." << std::endl;
    // Process all the node's meshes, if any
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
	// the node object only contains indices to index the actual objects in the scene. 
	// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
	aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
	m_Meshes.push_back(processMesh(mesh, scene));
    }
    // Then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
	processNode(node->mChildren[i], scene);
    }
 
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    for (unsigned int i = 0; i<mesh->mNumVertices; i++) {
	Vertex vertex;
	vertex.position.x = mesh->mVertices[i].x;
	vertex.position.y = mesh->mVertices[i].y;
	vertex.position.z = mesh->mVertices[i].z;
	if (mesh->HasNormals()) {
	    vertex.normal.x = mesh->mNormals[i].x;
	    vertex.normal.y = mesh->mNormals[i].y;
	    vertex.normal.z = mesh->mNormals[i].z;
	}
	/* NOTE: Assimp allows a model to have up to 8 different texture coordinates
	 * per vertex. Here, we only care about the first ones. */
	if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinate?
	{
	    vertex.texcoord.x = mesh->mTextureCoords[0][i].x;
	    vertex.texcoord.y = mesh->mTextureCoords[0][i].y;
	} else {
	    vertex.texcoord = glm::vec2(0.0f, 0.0f);
	}
	// No tangeant and bitangeant in vertex struct yet
	// if (mesh->HasTangentsAndBitangents()) {
	//     // tangent
	//     vector.x = mesh->mTangents[i].x;
	//     vector.y = mesh->mTangents[i].y;
	//     vector.z = mesh->mTangents[i].z;
	//     vertex.Tangent = vector;
	//     // bitangent
	//     vector.x = mesh->mBitangents[i].x;
	//     vector.y = mesh->mBitangents[i].y;
	//     vector.z = mesh->mBitangents[i].z;
	//     vertex.Bitangent = vector;
	// }
	vertices.push_back(vertex);
    }
    //process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
	aiFace face = mesh->mFaces[i];
	for (unsigned int j = 0; j < face.mNumIndices; j++)
	    indices.push_back(face.mIndices[j]);
    }
    // process material
    if (mesh->mMaterialIndex >= 0) {
	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	// 1. diffuse maps
	std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    return Mesh(vertices, indices, textures);
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial *mat, const aiTextureType& type,  const std::string& typeName) {
    std::vector<std::shared_ptr<Texture>> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
	aiString str;
	mat->GetTexture(type, i, &str);
	std::string fullPath = m_Directory + '/' + std::string(str.C_Str());

	// Reuse an already-loaded texture if the same file was requested before.
	bool skip = false;
	for (unsigned int j = 0; j < m_TexturesLoaded.size(); j++) {
	    if (m_TexturesLoaded[j]->GetFilePath() == fullPath) {
		textures.push_back(m_TexturesLoaded[j]); // shares ownership, no reload
		skip = true;
		break;
	    }
	}
	if (!skip) {
	    // Backpack diffuse/specular maps are 3-channel JPEGs -> GL_RGB.
	    // flipVertically=false because assimp already applied aiProcess_FlipUVs.
	    auto texture = std::make_shared<Texture>(fullPath, false, GL_RGB, false);
	    texture->SetType(typeName);
	    textures.push_back(texture);
	    m_TexturesLoaded.push_back(texture);
	}
    }
    return textures;
}
