#pragma once

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Moveable.h"

class Camera : public Moveable{
public:
    Camera();

    // Generate both matrix
    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();

    // Methods for view matrix
    void setPosition(glm::vec3& position);
    void setFocusPoint(glm::vec3& focus);
    void setUp(glm::vec3& up);
    // Movement
    void movePositionFixedDistanceAndFixedFocus(float magnitudeAngle, float latitude, float longitude);

    // Methods for projection matrix
    void setFOV(float fov);
    void setRatio(float ratio);
    void setNearClipping(float near);
    void setFarClipping(float far);

    
    

private:

    // Cache for Projection and View matrix
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;

    // Parameters for view matrix
    glm::vec3 m_focusPoint;
    glm::vec3 m_up;
    bool m_viewHasChanged = true;

    // Parameters for projection matrix
    float m_fov;
    float m_ratio;
    float m_near;
    float m_far;
    bool m_projectionHasChanged = true;
};

#endif