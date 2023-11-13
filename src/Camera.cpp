//
// Created by gauta on 12-11-2023.
//

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
    m_position = position;
    m_worldUp = up;
    m_movementSpeed = 2.5f;
    m_pitch = pitch;
    m_yaw = yaw;
    m_mouseSensitivity = 0.1f;
    m_zoom = 45.0f;
    update_euler_angles();
}

void Camera::update_euler_angles() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::process_keyboard(MovementDirection direction, float delta_time) {
    float velocity = m_movementSpeed * delta_time;
    if (direction == MovementDirection::Forward)
        m_position += m_front * velocity;
    if (direction == MovementDirection::Backward)
        m_position -= m_front * velocity;
    if (direction == MovementDirection::Left)
        m_position -= m_right * velocity;
    if (direction == MovementDirection::Right)
        m_position += m_right * velocity;
}

void Camera::process_mouse_movement(float x_offset, float y_offset) {
    x_offset *= m_mouseSensitivity;
    y_offset *= m_mouseSensitivity;

    m_yaw += x_offset;
    m_pitch += y_offset;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    else if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    update_euler_angles();
}

void Camera::process_mouse_scroll(float y_offset) {
    m_zoom -= y_offset;
    if (m_zoom < 1.0f)
        m_zoom = 1.0f;
    else if (m_zoom >  45.0f)
        m_zoom = 45.0f;
}
