#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <stb_image.h>
#include <stdexcept>

#include "GLErrorManager.hpp"
#include "VertexStruct.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/mesh.h"
#include "assimp/material.h"


class Model {
    public:
        Model(const std::string& path, bool gamma = false);

        ~Model() = default;

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;
        Model(Model&&) noexcept = default;
        Model& operator=(Model&&) noexcept = default;

        void Draw(Shader &shader);
    private:
        // model data
        std::vector<std::shared_ptr<Texture>> m_TexturesLoaded;
        std::vector<Mesh> m_Meshes;
        std::string m_Directory;
        bool m_GammaCorrection;


        void loadModel(const std::string& path);
        void processNode(aiNode *node, const aiScene *scene);
        /* Process a mesh from assimp, and return a `Mesh` class from it.
         * @param[in] mesh
         * @param[in] scene
         */
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, const aiTextureType& type, const std::string& typeName);

};
