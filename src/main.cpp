#include <glad/gl.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <array>
#include <fstream>
#include <optional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"
#include "Camera.h"

std::optional<float> last_x;
std::optional<float> last_y;
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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
    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, []([[maybe_unused]]GLFWwindow* glfw_window, const int32_t width, const int32_t height) {
       glViewport(0, 0, width, height);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* glfw_window, double x_pos_in, double y_pos_in) {
        if (!last_x.has_value() || !last_y.has_value()) {
            last_x = x_pos_in;
            last_y = y_pos_in;
        }
        float x_offset = static_cast<float>(x_pos_in) - last_x.value();
        float y_offset = last_y.value() - static_cast<float>(y_pos_in);

        last_x = x_pos_in;
        last_y = y_pos_in;
        camera.process_mouse_movement(x_offset, y_offset);
    });
    glfwSetScrollCallback(window, [](GLFWwindow* glfw_window, double x_offset, double y_offset) {
       camera.process_mouse_scroll(static_cast<float>(y_offset));
    });

    Shader lighting_shader("../src/shader.vert", "../src/lighting_shader.frag");
    Shader light_cube_shader("../src/shader.vert", "../src/white_shader.frag");

    constexpr std::array vertices = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    uint32_t vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    int32_t width, height, channels;
    uint8_t* data = stbi_load("../src/container.jpg", &width, &height, &channels, 0);
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
        std::cout << "Failed to load texture" << std::endl;
        glfwTerminate();
        return -1;
    }
    stbi_image_free(data);

    uint32_t light_vao;
    glGenVertexArrays(1, &light_vao);
    glBindVertexArray(light_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glm::vec3 light_pos(1.2f, 1.0f, 2.0f);

    float delta_time;
    float last_frame = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        auto current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.process_keyboard(MovementDirection::Forward, delta_time);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.process_keyboard(MovementDirection::Backward, delta_time);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.process_keyboard(MovementDirection::Left, delta_time);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.process_keyboard(MovementDirection::Right, delta_time);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the cube
        glBindVertexArray(vao);
        lighting_shader.use();
        int32_t model_location = glGetUniformLocation(lighting_shader.m_programID, "model");
        int32_t view_location = glGetUniformLocation(lighting_shader.m_programID, "view");
        int32_t projection_location = glGetUniformLocation(lighting_shader.m_programID, "projection");
        int32_t light_ambient = glGetUniformLocation(lighting_shader.m_programID, "light.ambient");
        int32_t light_diffuse = glGetUniformLocation(lighting_shader.m_programID, "light.diffuse");
        int32_t light_specular = glGetUniformLocation(lighting_shader.m_programID, "light.specular");
        int32_t light_position = glGetUniformLocation(lighting_shader.m_programID, "light.position");
        int32_t view_pos = glGetUniformLocation(lighting_shader.m_programID, "viewPos");
        int32_t ambient = glGetUniformLocation(lighting_shader.m_programID, "material.ambient");
        int32_t diffuse = glGetUniformLocation(lighting_shader.m_programID, "material.diffuse");
        int32_t specular = glGetUniformLocation(lighting_shader.m_programID, "material.specular");
        int32_t shininess = glGetUniformLocation(lighting_shader.m_programID, "material.shininess");

        glm::mat4 model(1.0f);
        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

        glm::mat4 view = camera.get_view_matrix();
        glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 projection = glm::perspective(glm::radians(camera.get_zoom()), 800.0f / 600.0f, 0.1f, 100.0f);
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

        glUniform3fv(light_ambient, 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
        glUniform3fv(light_diffuse, 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
        glUniform3fv(light_specular, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniform3fv(light_position, 1, glm::value_ptr(light_pos));
        glUniform3fv(view_pos, 1, glm::value_ptr(camera.get_position()));
        glUniform3fv(ambient, 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
        glUniform3fv(diffuse, 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
        glUniform3fv(specular, 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
        glUniform1f(shininess, 32.0f);

        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Render the light cube
        glBindVertexArray(light_vao);
        light_cube_shader.use();
        model_location = glGetUniformLocation(light_cube_shader.m_programID, "model");
        view_location = glGetUniformLocation(light_cube_shader.m_programID, "view");
        projection_location = glGetUniformLocation(light_cube_shader.m_programID, "projection");

        model = glm::mat4(1.0f);
        model = glm::translate(model, light_pos);
        model = glm::scale(model, glm::vec3(0.2f));

        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
