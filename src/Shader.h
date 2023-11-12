#pragma once

#include <cstdint>
#include <string>

class Shader {
public:
    uint32_t m_programID;

    Shader(const std::string& vertex_filename, const std::string& fragment_filename);

    void use() const;
};