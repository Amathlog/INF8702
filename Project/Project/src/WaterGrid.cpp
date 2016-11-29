#include "WaterGrid.h"
#include <iostream>
#include "Texture.hpp"

#ifndef M_PI
    #define M_PI 3.14159265359
#endif 

WaterGrid::WaterGrid(CNuanceurProg shader, glm::vec3 position, int subdivX, int subdivY, float height, float width) : Renderable(shader, position), 
                                                                                            m_subdivX(subdivX), m_subdivY(subdivY), m_height(height), m_width(width) {
    generateGrid();
    init();
    m_texture = Texture::loadBMP("textures/swimmingpool.bmp");
}

void WaterGrid::setCube(Cube* cube) {
    m_cube = cube;
}

void WaterGrid::init() {
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

        glGenBuffers(1, &m_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, m_vertexBufferData.size() * sizeof(GLfloat), &m_vertexBufferData[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &m_vertexNormalBuffer);

        glGenBuffers(1, &m_vertexHeightBuffer);
        refreshHeightsBuffer();

    glBindVertexArray(0);
}

void WaterGrid::refreshHeightsBuffer() {
    std::vector<GLfloat> heightBufferData(m_vertexBufferData.size() / 3);
    for (int i = 0; i < m_subdivX; i++) {
        for (int j = 0; j < m_subdivY; j++) {
            heightBufferData[i * m_subdivY * 6 + j * 6] = m_heights[i][j];
            heightBufferData[i * m_subdivY * 6 + j * 6 + 1] = m_heights[i+1][j];
            heightBufferData[i * m_subdivY * 6 + j * 6 + 2] = m_heights[i+1][j+1];
            heightBufferData[i * m_subdivY * 6 + j * 6 + 3] = m_heights[i][j];
            heightBufferData[i * m_subdivY * 6 + j * 6 + 4] = m_heights[i+1][j+1];
            heightBufferData[i * m_subdivY * 6 + j * 6 + 5] = m_heights[i][j+1];

            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18] = m_newNormals[i][j].x;
            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 1] = sqrt(1.0f - glm::dot(m_newNormals[i][j], m_newNormals[i][j]));
            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 2] = m_newNormals[i][j].y;

            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 3] = m_newNormals[i+1][j].x;
            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 4] = sqrt(1.0f - glm::dot(m_newNormals[i+1][j], m_newNormals[i+1][j]));
            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 5] = m_newNormals[i+1][j].y;

            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 6] = m_newNormals[i+1][j+1].x;
            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 7] = sqrt(1.0f - glm::dot(m_newNormals[i+1][j+1], m_newNormals[i + 1][j + 1]));
            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 8] = m_newNormals[i + 1][j + 1].y;


            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 9] = m_newNormals[i][j].x;
            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 10] = sqrt(1.0f - glm::dot(m_newNormals[i][j], m_newNormals[i][j]));
            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 11] = m_newNormals[i][j].y;

            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 12] = m_newNormals[i + 1][j + 1].x;
            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 13] = sqrt(1.0f - glm::dot(m_newNormals[i + 1][j + 1], m_newNormals[i + 1][j + 1]));
            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 14] = m_newNormals[i + 1][j + 1].y;

            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 15] = m_newNormals[i][j + 1].x;
            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 16] = sqrt(1.0f - glm::dot(m_newNormals[i][j + 1], m_newNormals[i][j + 1]));
            m_vertexNormalBufferData[i * m_subdivY * 18 + j * 18 + 17] = m_newNormals[i][j + 1].y;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexHeightBuffer);
        glBufferData(GL_ARRAY_BUFFER, heightBufferData.size() * sizeof(GLfloat), &heightBufferData[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexNormalBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_vertexNormalBufferData.size() * sizeof(GLfloat), &m_vertexNormalBufferData[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void WaterGrid::generateGrid() {
    m_vertexBufferData.resize(m_subdivX * m_subdivY * 6 * 3);
    m_vertexNormalBufferData.resize(m_subdivX * m_subdivY * 3 * 6);
    float xOffset = m_width / m_subdivX;
    float yOffset = m_height / m_subdivY;
    float xStart = m_position.x - m_width / 2.0f;
    float yStart = m_position.y - m_height / 2.0f;

    for (int i = 0; i < m_subdivX; i++) {
        for (int j = 0; j < m_subdivY; j++) {
            m_vertexBufferData[i*m_subdivY * 18 + 18 * j] = xStart + i*xOffset;
            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 1] = yStart + j*yOffset;
            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 2] = m_position.z;

            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 3] = xStart + (i+1)*xOffset;
            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 4] = yStart + j*yOffset;
            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 5] = m_position.z;

            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 6] = xStart + (i+1)*xOffset;
            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 7] = yStart + (j+1)*yOffset;
            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 8] = m_position.z;


            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 9] = xStart + i*xOffset;
            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 10] = yStart + j*yOffset;
            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 11] = m_position.z;

            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 12] = xStart + (i+1)*xOffset;
            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 13] = yStart + (j+1)*yOffset;
            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 14] = m_position.z;

            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 15] = xStart + i*xOffset;
            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 16] = yStart + (j+1)*yOffset;
            m_vertexBufferData[i*m_subdivY * 18 + 18 * j + 17] = m_position.z;            
        }
    }

    m_heights.resize(m_subdivX+1);
    m_velocities.resize(m_subdivX + 1);
    m_newHeights.resize(m_subdivX + 1);
    m_newNormals.resize(m_subdivX + 1);
    for (int i = 0; i < m_subdivX + 1; i++) {
        m_heights[i].resize(m_subdivY + 1);
        m_velocities[i].resize(m_subdivY + 1);
        m_newHeights[i].resize(m_subdivY + 1);
        m_newNormals[i].resize(m_subdivY + 1);
        for (int j = 0; j < m_subdivY + 1; j++) {
            m_heights[i][j] = 0.0f;
            m_velocities[i][j] = 0.0f;
            m_newHeights[i][j] = 0.0f;
            m_newNormals[i][j] = glm::vec2(0.0f, 1.0f);
        }
    }
}


void WaterGrid::draw(Camera& camera) {
    // Handle the MVP matrix
    Renderable::prepareDrawing(camera);
    
    computeNextStep();
    refreshHeightsBuffer();
    
    // Get a handle for our "incomingRay" uniform
    GLuint handle = glGetUniformLocation(m_shader.getProg(), "eye");
    glm::vec3 eye = camera.getPosition();
    glUniform3fv(handle, 1, &eye[0]);

    handle = glGetUniformLocation(m_shader.getProg(), "halfEdgeLength");
    glUniform1f(handle, m_cube->getEdgeLength()/2.0f);

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
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexHeightBuffer);

        glVertexAttribPointer(
            2,                  // attribute 2.
            1,                  // size
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
}

void WaterGrid::computeNextStep() {
    for (int i = 1; i < m_subdivX; i++) {
        for (int j = 1; j < m_subdivY; j++) {
            // Heights
            float f = 0.25f * (m_heights[i + 1][j] + m_heights[i - 1][j] + m_heights[i][j + 1] + m_heights[i][j - 1]);
            m_velocities[i][j] += (f - m_heights[i][j]) * 2.0f;
            m_velocities[i][j] *= 0.995;
            m_newHeights[i][j] = m_heights[i][j] + m_velocities[i][j];

            // Normals
            glm::vec3 dx{ 0.0f , m_width / m_subdivX , m_newHeights[i + 1][j] - m_newHeights[i][j] };
            glm::vec3 dy{ m_height / m_subdivY , 0.0f,  m_newHeights[i][j] - m_newHeights[i][j+1] };
            glm::vec3 aux = glm::normalize(glm::cross(dy, dx));
            m_newNormals[i][j].x = aux.x;
            m_newNormals[i][j].y = aux.z;
        }
    }
    m_heights = m_newHeights;
}


void WaterGrid::addPerturbation() {
    m_heights[m_subdivX / 2][m_subdivY / 2] = -0.2f;
}

