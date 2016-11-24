#include "Cube.h"
#include <iostream>
#include <ctime>

Cube::Cube(CNuanceurProg shader, glm::vec3 position, float edgeLength) : Renderable(shader, position), m_edgeLength(edgeLength) {
    init();
}

void Cube::init() {

    generateVertex();

    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

        glGenBuffers(1, &m_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, m_vertexBufferData.size() * sizeof(GLfloat), &m_vertexBufferData[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &m_vertexColorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexColorBuffer);
            glBufferData(GL_ARRAY_BUFFER, m_vertexColorBufferData.size() * sizeof(GLfloat), &m_vertexColorBufferData[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &m_vertexIndiceBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexIndiceBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_vertexIndiceData), m_vertexIndiceData, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Cube::draw(Camera& camera) {

    // Handle the MVP matrix
    Renderable::prepareDrawing(camera);

    glBindVertexArray(m_vertexArrayID);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

        glVertexAttribPointer(
            0,                          // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                          // size
            GL_FLOAT,                   // type
            GL_FALSE,                   // normalized?
            0,                          // stride
            (void*)0);                  // array buffer offset
        
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexColorBuffer);

        glVertexAttribPointer(
            1,                  // attribute 1.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
            );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexIndiceBuffer);

        glDrawElements(
            GL_TRIANGLES,                   // mode
            36,                             // count
            GL_UNSIGNED_SHORT,              // type
            (void*)0);                      // element array buffer offset

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Cube::generateVertex() {
    GLfloat offset = m_edgeLength / 2.0f;
    GLfloat px = m_position.x;
    GLfloat py = m_position.y;
    GLfloat pz = m_position.z;

    m_vertexBufferData = std::vector<GLfloat>{ 
        px - offset, py + offset, pz - offset,
        px + offset, py + offset, pz - offset,
        px + offset, py + offset, pz + offset,
        px - offset, py + offset, pz + offset,
        px - offset, py - offset, pz - offset,
        px + offset, py - offset, pz - offset,
        px + offset, py - offset, pz + offset,
        px - offset, py - offset, pz + offset };

    m_vertexColorBufferData.resize(12 * 3 * 3);

    srand(time(NULL));
    for (int i = 0; i < m_vertexColorBufferData.size(); i++) {
        m_vertexColorBufferData[i] = float(rand()) / RAND_MAX;
    }

}