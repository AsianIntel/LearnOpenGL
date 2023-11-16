#pragma once

#include <glm/glm.hpp>
#include <cstdint>
#include <utility>
#include <vector>

#include "Shader.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

enum class TextureType {
    Diffuse,
    Specular
};

struct Texture {
    uint32_t id;
    TextureType type;
    std::string path;
};

class Mesh {
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    std::vector<Texture> m_textures;

    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;

public:
    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
        : m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_textures(std::move(textures)) {
        setup_mesh();
    }

    void draw(const Shader& shader);

private:
    void setup_mesh();
};

constexpr static std::string to_string(TextureType type) {
    switch (type) {
        case TextureType::Diffuse: return "texture_diffuse";
        case TextureType::Specular: return "texture_specular";
    }
}