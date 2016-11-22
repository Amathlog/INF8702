#include "Camera.h"
#include "glm\gtc\matrix_transform.hpp"

// Default values
Camera::Camera() {
    m_position = glm::vec3(3.0f, 3.0f, 3.0f);
    m_focusPoint = glm::vec3(0.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);

    m_fov = 45.0f;
    m_ratio = 1.0f;
    m_near = 0.1f;
    m_far = 100.0f;
}

glm::mat4 Camera::getProjectionMatrix() {
    if (m_projectionHasChanged) {
        m_projectionMatrix = glm::perspective(m_fov, m_ratio, m_near, m_far);
        m_projectionHasChanged = false;
    }
    return m_projectionMatrix;
}

glm::mat4 Camera::getViewMatrix() {
    if (m_viewHasChanged) {
        m_viewMatrix = glm::lookAt(m_position, m_focusPoint, m_up);
        m_viewHasChanged = false;
    }
    return m_viewMatrix;
}


void Camera::setPosition(glm::vec3& position) {
    m_viewHasChanged = true;
    Moveable::setPosition(position);
}

void Camera::setFocusPoint(glm::vec3& focus) {
    m_viewHasChanged = true;
    m_focusPoint = focus;
}

void Camera::setUp(glm::vec3& up) {
    m_viewHasChanged = true;
    m_up = up;
}

void Camera::setFOV(float fov) {
    m_projectionHasChanged = true;
    m_fov = fov;
}

void Camera::setRatio(float ratio) {
    m_projectionHasChanged = true;
    m_ratio = ratio;
}

void Camera::setNearClipping(float near) {
    m_projectionHasChanged = true;
    m_near = near;
}

void Camera::setFarClipping(float far) {
    m_projectionHasChanged = true;
    m_far = far;
}