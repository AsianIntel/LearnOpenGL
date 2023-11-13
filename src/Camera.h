#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class MovementDirection {
    Forward,
    Backward,
    Left,
    Right
};

class Camera {
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_worldUp;
    glm::vec3 m_right;
    float m_yaw;
    float m_pitch;
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;

public:
    Camera(glm::vec3 position, glm::vec3 up, float yaw = -90.0f, float pitch = 0.0f);

    [[nodiscard]] glm::mat4 get_view_matrix() const { return glm::lookAt(m_position, m_position + m_front, m_up); }
    [[nodiscard]] float get_zoom() const { return m_zoom; }
    [[nodiscard]] glm::vec3 get_position() const { return m_position; }

    void process_keyboard(MovementDirection direction, float delta_time);
    void process_mouse_movement(float x_offset, float y_offset);
    void process_mouse_scroll(float y_offset);

private:
    void update_euler_angles();
};