#pragma once

#include <string>
#include <vector>
#include <assimp/scene.h>

#include "Mesh.h"

class Model {
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    std::vector<Texture> m_loadedTextures;

public:
    Model(const std::string& path) { load_model(path); }

    void draw(const Shader& shader);

private:
    void load_model(const std::string& path);
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> load_material_textures(aiMaterial* material, aiTextureType texture_type, TextureType texture_type_name);
};