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
#include "Model.h"

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

    stbi_set_flip_vertically_on_load(true);

    Shader lighting_shader("../src/shader.vert", "../src/model_loading.frag");
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
    glBindVertexArray(0);

    uint32_t light_vao;
    glGenVertexArrays(1, &light_vao);
    glBindVertexArray(light_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
    glBindVertexArray(0);

    glm::vec3 light_pos(1.2f, 1.0f, 2.0f);

    glm::vec3 point_light_positions[] = {
            glm::vec3( 0.7f, 0.2f, 2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f, 2.0f, -12.0f),
            glm::vec3( 0.0f, 0.0f, -3.0f)
    };

    Model backpack_model("../src/backpack/backpack.obj");

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

        glm::mat4 view = camera.get_view_matrix();
        glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 projection = glm::perspective(glm::radians(camera.get_zoom()), 800.0f / 600.0f, 0.1f, 100.0f);
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "viewPos"), 1, glm::value_ptr(camera.get_position()));
        glUniform1i(glGetUniformLocation(lighting_shader.m_programID, "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(lighting_shader.m_programID, "material.specular"), 1);
        glUniform1f(glGetUniformLocation(lighting_shader.m_programID, "material.shininess"), 32.0f);

        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "directional_light.direction"), 1, glm::value_ptr(glm::vec3(-0.2f, -1.0f, -0.3f)));
        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "directional_light.ambient"), 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "directional_light.diffuse"), 1, glm::value_ptr(glm::vec3(0.4f, 0.4f, 0.4f)));
        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "directional_light.specular"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));

        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "pointLights[0].position"), 1, glm::value_ptr(point_light_positions[0]));
        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "pointLights[0].ambient"), 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "pointLights[0].diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "pointLights[0].specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniform1f(glGetUniformLocation(lighting_shader.m_programID, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lighting_shader.m_programID, "pointLights[0].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lighting_shader.m_programID, "pointLights[0].quadratic"), 0.032f);

        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "pointLights[1].position"), 1, glm::value_ptr(point_light_positions[1]));
        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "pointLights[1].ambient"), 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "pointLights[1].diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "pointLights[1].specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniform1f(glGetUniformLocation(lighting_shader.m_programID, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lighting_shader.m_programID, "pointLights[1].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lighting_shader.m_programID, "pointLights[1].quadratic"), 0.032f);

        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "pointLights[2].position"), 1, glm::value_ptr(point_light_positions[0]));
        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "pointLights[2].ambient"), 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "pointLights[2].diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "pointLights[2].specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniform1f(glGetUniformLocation(lighting_shader.m_programID, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lighting_shader.m_programID, "pointLights[2].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lighting_shader.m_programID, "pointLights[2].quadratic"), 0.032f);

        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "pointLights[3].position"), 1, glm::value_ptr(point_light_positions[0]));
        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "pointLights[3].ambient"), 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "pointLights[3].diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
        glUniform3fv(glGetUniformLocation(lighting_shader.m_programID, "pointLights[3].specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniform1f(glGetUniformLocation(lighting_shader.m_programID, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lighting_shader.m_programID, "pointLights[3].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lighting_shader.m_programID, "pointLights[3].quadratic"), 0.032f);

        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
        backpack_model.draw(lighting_shader);

        // Render the light cube
        glBindVertexArray(light_vao);
        light_cube_shader.use();
        model_location = glGetUniformLocation(light_cube_shader.m_programID, "model");
        view_location = glGetUniformLocation(light_cube_shader.m_programID, "view");
        projection_location = glGetUniformLocation(light_cube_shader.m_programID, "projection");

        glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
        for (auto point_light_position : point_light_positions) {
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, point_light_position);
            model = glm::scale(model, glm::vec3(0.2f));

            glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
