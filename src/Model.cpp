#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include <stb_image.h>
#include <glad/gl.h>

uint32_t texture_from_file(const char* path, const std::string& directory) {
    std::string filename = path;
    filename = directory + '/' + filename;

    uint32_t texture_id;
    glGenTextures(1, &texture_id);

    int32_t width, height, n_channels;
    uint8_t* data = stbi_load(filename.c_str(), &width, &height, &n_channels, 0);
    if (data) {
        GLenum format;
        if (n_channels == 1)
            format = GL_RED;
        else if (n_channels == 3)
            format = GL_RGB;
        else if (n_channels == 4)
            format = GL_RGBA;
        else {
            std::cout << "Unsupported texture format" << std::endl;
            __debugbreak();
        }

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        __debugbreak();
    }

    return texture_id;
}

void Model::draw(const Shader &shader) {
    for (auto& mesh: m_meshes) {
        mesh.draw(shader);
    }
}

void Model::load_model(const std::string &path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "assimp error: " << importer.GetErrorString() << std::endl;
        __debugbreak();
    }
    m_directory = path.substr(0, path.find_last_of('/'));

    process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode *node, const aiScene *scene) {
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(process_mesh(mesh, scene));
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene);
    }
}

Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    // Process Vertices
    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex {
            .position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
            .normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
            .tex_coords = (mesh->mTextureCoords[0]) ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f, 0.0f)
        };
        vertices.emplace_back(vertex);
    }

    // Process indices
    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++) {
            indices.emplace_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

        std::vector<Texture> specular_maps = load_material_textures(material, aiTextureType_SPECULAR, TextureType::Specular);
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    }

    return {vertices, indices, textures};
}

std::vector<Texture> Model::load_material_textures(aiMaterial *material, aiTextureType texture_type, TextureType texture_type_name) {
    std::vector<Texture> textures;
    for (uint32_t i = 0; i < material->GetTextureCount(texture_type); i++) {
        aiString str;
        material->GetTexture(texture_type, i, &str);

        bool skip = false;
        for (auto& m_loadedTexture : m_loadedTextures) {
            if (std::strcmp(m_loadedTexture.path.data(), str.C_Str()) == 0) {
                textures.push_back(m_loadedTexture);
                skip = true;
                break;
            }
        }

        if (!skip) {
            Texture texture {
                    .id = texture_from_file(str.C_Str(), m_directory),
                    .type = texture_type_name,
                    .path = str.C_Str()
            };
            textures.push_back(texture);
            m_loadedTextures.push_back(texture);
        }
    }

    return textures;
}
