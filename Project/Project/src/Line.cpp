#include "Line.h"

Line::Line(CNuanceurProg shader, glm::vec3 direction, glm::vec3 position, glm::vec3 color, float length) : Renderable(shader, position), m_length(length), m_direction(direction), m_color(color) {
    init();
}

void Line::init() {

    std::vector<GLfloat> vertrices(6);

    vertrices[0] = m_position[0];
    vertrices[1] = m_position[1];
    vertrices[2] = m_position[2];
    vertrices[3] = m_position[0] + m_length * m_direction[0];
    vertrices[4] = m_position[1] + m_length * m_direction[1];
    vertrices[5] = m_position[2] + m_length * m_direction[2];

    std::vector<GLfloat> color(6);

    color[0] = m_color[0];
    color[1] = m_color[1];
    color[2] = m_color[2];
    color[3] = m_color[0];
    color[4] = m_color[1];
    color[5] = m_color[2];

    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertrices.size() * sizeof(GLfloat), &vertrices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_vertexColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(GLfloat), &color[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Line::draw(Camera& camera) {

    // Handle the MVP matrix
    Renderable::prepareDrawing(camera);

    glBindVertexArray(m_vertexArrayID);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_LINE_STIPPLE);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexColorBuffer);

    glVertexAttribPointer(
        1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );

    glDrawArrays(GL_LINES, 0, 2);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glDisable(GL_LINE_STIPPLE);
    glDisable(GL_LINE_SMOOTH);

    glBindVertexArray(0);
}