#include "../include/Control.h"
#include "Scene.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "WaterGrid.h"

Control::Control() : m_window(nullptr), m_scene(nullptr) {

}

void Control::zoomImpl(GLFWwindow* window, double xoffset, double yoffset) {
    if (m_window == nullptr)
        throw "Window has not been set in control before using it";
    m_scene->getCamera().zoom(-1 * yoffset * m_cameraSensibility);
}

void Control::rotate() {
    if (m_window == nullptr)
        throw "Window has not been set in control before using it";

    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double xPos, yPos;
        glfwGetCursorPos(m_window, &xPos, &yPos);
        if (m_savePositionMouse == glm::vec2(-1, -1)) {
            m_savePositionMouse.x = xPos;
            m_savePositionMouse.y = yPos;
        }
        else {
            m_savePositionMouse = m_currPositionMouse;
        }

        m_currPositionMouse.x = xPos;
        m_currPositionMouse.y = yPos;

        float latitude = m_savePositionMouse.x - m_currPositionMouse.x;
        float longitude = m_savePositionMouse.y - m_currPositionMouse.y;

        m_scene->getCamera().movePositionFixedDistanceAndFixedFocus(m_cameraSensibility, latitude, longitude);
    }
    else if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        m_savePositionMouse = glm::vec2(-1, -1);
    }
}

void Control::keyRotate() {
    if (m_window == nullptr)
        throw "Window has not been set in control before using it";

    if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
        m_scene->getCamera().movePositionFixedDistanceAndFixedFocus(2.0f*m_cameraSensibility, 0.0f, -1.0f);
    else if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
        m_scene->getCamera().movePositionFixedDistanceAndFixedFocus(2.0f*m_cameraSensibility, 0.0f, 1.0f);
    else if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
        m_scene->getCamera().movePositionFixedDistanceAndFixedFocus(2.0f*m_cameraSensibility, -1.0f, 0.0f);
    else if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        m_scene->getCamera().movePositionFixedDistanceAndFixedFocus(2.0f*m_cameraSensibility, 1.0f, 0.0f);
}

void Control::setWindowAndScene(GLFWwindow* window, Scene* scene) {
    m_window = window;
    m_scene = scene;
    glfwSetScrollCallback(window, &Control::zoom);
}

void Control::addWaterPerturbation() {
    if (glfwGetKey(m_window, GLFW_KEY_P) == GLFW_PRESS) {
        m_grid->addPerturbation(glm::vec3(0.0f, 0.0f, 0.0f));
    }
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        return;
        std::cout << "Click : " << getMousePositionInWorldCoordinates().x << ", " << getMousePositionInWorldCoordinates().y << ", " << getMousePositionInWorldCoordinates().z << std::endl;
        m_grid->addPerturbation(getMousePositionInWorldCoordinates());
    }
}

void Control::setGrid(WaterGrid* grid) {
    m_grid = grid;
}

void Control::processEvents() {
    rotate();
    keyRotate();
    addWaterPerturbation();
}

glm::vec3 Control::getMousePositionInWorldCoordinates() {
    Camera camera = m_scene->getCamera();

    int width, height;
    glfwGetWindowSize(m_window, &width, &height);

    double xPos, yPos;
    glfwGetCursorPos(m_window, &xPos, &yPos);

    GLfloat depth = 0.0f;
    glReadPixels(floor(xPos), floor(height - yPos), 1, 1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth);

    std::cout << "z = " << depth << std::endl;

    glm::mat4 invVP = glm::inverse(camera.getViewMatrix()) *  glm::inverse(camera.getProjectionMatrix());
    glm::vec4 cursor{ 2.0f * float(xPos) / width - 1.0f, 2.0f * float(yPos) / height - 1.0f, depth, 1.0f };

    glm::vec4 pos = invVP * cursor;

    pos.w = 1.0 / pos.w;

    pos.x *= pos.w;
    pos.y *= pos.w;
    pos.z *= pos.w;

    return glm::vec3(pos);
}
