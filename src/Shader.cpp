#include "Shader.h"
#include <glad/gl.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <optional>
#include <array>

std::string read_file(const std::string& filename) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(filename);

    std::stringstream file_stream;
    file_stream << file.rdbuf();

    file.close();

    std::string file_data = file_stream.str();
    return file_data;
}

std::optional<uint32_t> read_shader(const std::string& code, const GLenum type) {
    const uint32_t shader = glCreateShader(type);
    const char* shader_data = code.c_str();
    glShaderSource(shader, 1, &shader_data, nullptr);
    glCompileShader(shader);

    int32_t success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::array<char, 512> shader_error_log{};
        glGetShaderInfoLog(shader, 512, nullptr, shader_error_log.data());
        std::cout << "Error compiling shader: " << shader_error_log.data() << std::endl;
        return {};
    }

    return shader;
}

Shader::Shader(const std::string& vertex_filename, const std::string& fragment_filename) {
    std::string vertex_code = read_file(vertex_filename);
    std::string fragment_code = read_file(fragment_filename);

    const uint32_t vertex_shader = read_shader(vertex_code, GL_VERTEX_SHADER).value();
    const uint32_t fragment_shader = read_shader(fragment_code, GL_FRAGMENT_SHADER).value();

    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertex_shader);
    glAttachShader(m_programID, fragment_shader);
    glLinkProgram(m_programID);
    {
        int32_t success;
        glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
        if (!success) {
            std::array<char, 512> shader_error_log{};
            glGetProgramInfoLog(m_programID, 512, nullptr, shader_error_log.data());
            std::cout << "Error linking shader: " << shader_error_log.data() << std::endl;
            __debugbreak();
        }
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::use() const {
    glUseProgram(m_programID);
}
