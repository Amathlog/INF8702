#include "Cube.h"
#include <iostream>
#include <ctime>
#include "Texture.hpp"
#include "WaterGrid.h"


Cube::Cube(CNuanceurProg shader, glm::vec3 position, float edgeLength) : Renderable(shader, position), m_edgeLength(edgeLength) {
    init();
}

void Cube::init() {

    generateBuffers();
    m_texture = Texture::loadBMP("textures/swimmingpool.bmp");

    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

        glGenBuffers(1, &m_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, m_vertexBufferData.size() * sizeof(GLfloat), &m_vertexBufferData[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &m_vertexNormalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexNormalBuffer);
            glBufferData(GL_ARRAY_BUFFER, m_vertexNormalBufferData.size() * sizeof(GLfloat), &m_vertexNormalBufferData[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &m_vertexTexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexTexBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_vertexTexBufferData.size() * sizeof(GLfloat), &m_vertexTexBufferData[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Cube::draw(Camera& camera) {

    // Should be disable in case of some faces not rendering. 
    //glDisable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    // Handle the MVP matrix
    Renderable::prepareDrawing(camera);

    // Get our eye
    glm::vec3 eye = camera.getEye();

    // Get a handle for our "eye" uniform
    GLuint eyeHandle = glGetUniformLocation(m_shader.getProg(), "eye");
    glUniform3fv(eyeHandle, 1, &eye[0]);

    // Get a handle for our "heightMax" uniform
    GLuint handle = glGetUniformLocation(m_shader.getProg(), "heightMax");
    glUniform1f(handle, m_watergrid->getPosition().z);

    glBindTexture(GL_TEXTURE_2D, m_texture);

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
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexNormalBuffer);

        glVertexAttribPointer(
            1,                  // attribute 1.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
            );

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexTexBuffer);

        glVertexAttribPointer(
            2,                  // attribute 2.
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
            );

        glDrawArrays(GL_TRIANGLES, 0, m_vertexBufferData.size());

        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

    glBindVertexArray(0);

    glDisable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);
}

void Cube::generateBuffers() {
    GLfloat offset = m_edgeLength / 2.0f;
    GLfloat px = m_position.x;
    GLfloat py = m_position.y;
    GLfloat pz = m_position.z;

    std::vector<GLfloat> verteces{
        px + offset, py - offset, pz + offset,
        px + offset, py + offset, pz + offset,
        px - offset, py + offset, pz + offset,
        px - offset, py - offset, pz + offset,
        px + offset, py - offset, pz - offset,
        px + offset, py + offset, pz - offset,
        px - offset, py + offset, pz - offset,
        px - offset, py - offset, pz - offset };

    std::vector<GLfloat> normales{
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, -1.0f, 0.0f,
    };

    m_vertexBufferData.resize(10 * 3 * 3);
    m_vertexNormalBufferData.resize(10 * 3 * 3);
    m_vertexTexBufferData.resize(10 * 3 * 2);

    for (int i = 0; i < 30; i++) {
        m_vertexBufferData[3 * i] = verteces[3 * m_vertexIndiceData[i]];
        m_vertexBufferData[3 * i + 1] = verteces[3 * m_vertexIndiceData[i] + 1];
        m_vertexBufferData[3 * i + 2] = verteces[3 * m_vertexIndiceData[i] + 2];
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 3; k++) {
                m_vertexNormalBufferData[5 * i + 3 * j + k] = normales[3*j + k];
            }
            
        }
    }

    for (int i = 0; i < 5; i++) {
        m_vertexTexBufferData[12 * i] = 0.0f;
        m_vertexTexBufferData[12 * i + 1] = 1.0f;
        m_vertexTexBufferData[12 * i + 2] = 0.0f;
        m_vertexTexBufferData[12 * i + 3] = 0.0f;
        m_vertexTexBufferData[12 * i + 4] = 1.0f;
        m_vertexTexBufferData[12 * i + 5] = 1.0f;
        m_vertexTexBufferData[12 * i + 6] = 1.0f;
        m_vertexTexBufferData[12 * i + 7] = 1.0f;
        m_vertexTexBufferData[12 * i + 8] = 0.0f;
        m_vertexTexBufferData[12 * i + 9] = 0.0f;
        m_vertexTexBufferData[12 * i + 10] = 1.0f;
        m_vertexTexBufferData[12 * i + 11] = 0.0f;
    }

}

float Cube::getEdgeLength() {
    return m_edgeLength;
}

void Cube::setWaterGrid(WaterGrid* grid) {
    m_watergrid = grid;
}