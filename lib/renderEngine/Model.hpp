#pragma once

#include <vector>
#include <iostream>

#include "VertexStruct.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

class Model {
    public:
        Model(char *path) {
            loadModel(path);
        }
        void Draw(Shader &shader);
    private:
        // model data
        std::vector<Mesh> m_Meshes;
        std::string m_Directory;

        void loadModel(const std::string& path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

};
