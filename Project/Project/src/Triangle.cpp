#include "Triangle.h"

Triangle::Triangle(CNuanceurProg shader) : Renderable(shader){
    init();
}

void Triangle::init() {
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexBufferData), m_vertexBufferData, GL_STATIC_DRAW);

    glGenBuffers(1, &m_vertexColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexColorBufferData), m_vertexColorBufferData, GL_STATIC_DRAW);
}

void Triangle::draw(Camera& camera) {

    // Handle the MVP matrix
    Renderable::prepareDrawing(camera);

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
        1,                  // attribute 1.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );


    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}