#ifndef _CONTROL_H_
#define _CONTROL_H_

// Include GLM
#include <glm/vec2.hpp>

class Scene;
struct GLFWwindow;

class Control {
public:

    static Control& getInstance() {
        static Control instance;
        return instance;
    }
    static void zoom(GLFWwindow* window, double xoffset, double yoffset) {
        getInstance().zoomImpl(window, xoffset, yoffset);
    }
    void zoomImpl(GLFWwindow* window, double xoffset, double yoffset);
    void rotate();
    void keyRotate();
    void setWindowAndScene(GLFWwindow* window, Scene* scene);
private:
    Control();
    GLFWwindow* m_window;
    Scene* m_scene;
    // Mouse positions
    glm::vec2 m_savePositionMouse = glm::vec2(-1, -1);
    glm::vec2 m_currPositionMouse;

    const float m_cameraSensibility = 0.5f;
};

#endif
