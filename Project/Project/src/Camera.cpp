#include "Camera.h"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"
#include <iostream>

#define PI 3.14159265359f

// Default values
Camera::Camera() {
    m_position = glm::vec3(3.0f, 3.0f, 3.0f);
    m_focusPoint = glm::vec3(0.0f);
    m_up = glm::vec3(0.0f, 0.0f, 1.0f);

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

void Camera::movePositionFixedDistanceAndFixedFocus(float magnitudeAngle, float latitude, float longitude) {
    m_viewHasChanged = true;
    glm::vec3 towards = m_position - m_focusPoint;
    float r = glm::distance(m_position, m_focusPoint);

    float phi = atan2f(towards.y, towards.x);
    float theta = acos(towards.z / r);

    phi += magnitudeAngle * latitude * PI / 180.0f;
    theta += magnitudeAngle * longitude * ((phi < 0) ? -1.0f : 1.0f) * PI / 180.0f;
    if (theta > PI || theta < 0.0f) {
        m_up *= -1.0f;
    }
    towards.x = r * sin(theta) * cos(phi);
    towards.y = r * sin(theta) * sin(phi);
    towards.z = r * cos(theta);

    m_position = towards + m_focusPoint;
}

void Camera::zoom(float value) {
    m_viewHasChanged = true;
    glm::vec3 towards = glm::normalize(m_position - m_focusPoint);
    if (glm::distance(m_position, m_focusPoint) < abs(value) && value < 0) {
        return;
    }
    m_position = m_position + towards * value;
}

glm::vec3 Camera::getEye() {
    return glm::normalize(m_focusPoint - m_position);
}