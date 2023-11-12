#include <glad/gl.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <array>
#include <cstdint>
#include <vector>
#include <fstream>
#include <optional>

std::vector<char> read_file(const std::string& filename) {
    std::ifstream file(filename);
    file.seekg(0, std::ios::end);

    const uint32_t file_length = file.tellg();
    file.seekg(0);

    std::vector<char> file_data(file_length);
    file.read(file_data.data(), file_length);

    return file_data;
}

std::optional<uint32_t> read_shader(const std::string& filename, const GLenum type) {
    const std::vector<char> shader_source = read_file(filename);
    const uint32_t shader = glCreateShader(type);
    const char* vertex_shader_data = shader_source.data();
    glShaderSource(shader, 1, &vertex_shader_data, nullptr);
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

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to initialize GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cout << "Failed to initialize Glad" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, []([[maybe_unused]]GLFWwindow* glfw_window, const int32_t width, const int32_t height) {
       glViewport(0, 0, width, height);
    });

    const std::optional<uint32_t> vertex_shader = read_shader("../src/shader.vert", GL_VERTEX_SHADER);
    if (!vertex_shader.has_value()) {
        glfwTerminate();
        return -1;
    }
    const std::optional<uint32_t> fragment_shader = read_shader("../src/shader.frag", GL_FRAGMENT_SHADER);
    if (!fragment_shader.has_value()) {
        glfwTerminate();
        return -1;
    }

    const uint32_t shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader.value());
    glAttachShader(shader_program, fragment_shader.value());
    glLinkProgram(shader_program);
    {
        int32_t success;
        glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
        if (!success) {
            std::array<char, 512> shader_error_log{};
            glGetProgramInfoLog(shader_program, 512, nullptr, shader_error_log.data());
            std::cout << "Error linking shader: " << shader_error_log.data() << std::endl;
            glfwTerminate();
            return -1;
        }
    }
    glDeleteShader(vertex_shader.value());
    glDeleteShader(fragment_shader.value());

    constexpr std::array vertices = {
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f // top left
    };
    constexpr std::array indices = {
        0, 1, 3,
        1, 2, 3
    };

    uint32_t vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    uint32_t ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
