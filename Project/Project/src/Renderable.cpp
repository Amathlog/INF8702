#include "Renderable.h"

void Renderable::prepareDrawing(Camera& camera) {
    // Activate our shader
    m_shader.activer();

    // Compute the MVP matrix
    glm::mat4 MVP = camera.getProjectionMatrix() * camera.getViewMatrix() * getTransform();

    // Get a handle for our "MVP" uniform
    GLuint mvpHandle = glGetUniformLocation(m_shader.getProg(), "MVP");

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &MVP[0][0]);

    // Next is handled in each class
}

