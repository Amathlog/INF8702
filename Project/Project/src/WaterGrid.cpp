#include "WaterGrid.h"
#include <iostream>

#ifndef M_PI
    #define M_PI 3.14159265359
#endif 

#define GPU 0

WaterGrid::WaterGrid(CNuanceurProg shader, glm::vec3 position, int subdivX, int subdivY, float height, float width) : Renderable(shader, position), 
                                                                                            m_subdivX(subdivX), m_subdivY(subdivY), m_height(height), m_width(width),
                                                                                            m_heightMapShader{ "shaders/heightMap.vert", "shaders/heightMap.frag", true },
                                                                                            m_perturbationShader{ "shaders/addPerturbation.vert", "shaders/addPerturbation.frag", true } {
    generateGrid();
    init();
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

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);


    std::vector<GLfloat> textureInit((m_subdivX + 1) * (m_subdivY + 1) * 4);
    for (int i = 0; i < (m_subdivX + 1) * (m_subdivY + 1); i++) {
        textureInit[i * 4] = 0.0f; // Height = 0
        textureInit[i * 4 + 1] = 0.0f; // Velocity = 0
        textureInit[i * 4 + 2] = 0.0f; // Normal x = 0
        textureInit[i * 4 + 3] = 1.0f; // Normal z = 1
    }
    textureInit[((m_subdivX + 1) * (m_subdivY + 1) / 2 + (m_subdivY + 1) / 2) * 4 + 4*50] = 1.0f;

        glGenTextures(1, &m_heightMapTextureRead);
        glBindTexture(GL_TEXTURE_2D, m_heightMapTextureRead);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_subdivX + 1, m_subdivY + 1, 0, GL_RGBA, GL_FLOAT, &textureInit[0]);           

        glBindTexture(GL_TEXTURE_2D, 0);

        glGenTextures(1, &m_heightMapTextureWrite);
            glBindTexture(GL_TEXTURE_2D, m_heightMapTextureWrite);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_subdivX + 1, m_subdivY + 1, 0, GL_RGBA, GL_FLOAT, &textureInit[0]);

        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_heightMapTextureWrite, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR FRAMEBUFFER" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
}

void WaterGrid::refreshTexture() {
    std::vector<GLfloat> textureInit((m_subdivX + 1) * (m_subdivY + 1) * 4);
    for (int i = 0; i < (m_subdivX + 1) * (m_subdivY + 1); i++) {
        textureInit[i * 4] = m_heights[i / (m_subdivY+1)][i % (m_subdivY + 1)];
        textureInit[i * 4 + 1] = m_velocities[i / (m_subdivY + 1)][i % (m_subdivY + 1)]; 
        textureInit[i * 4 + 2] = m_newNormals[i / (m_subdivY + 1)][i % (m_subdivY + 1)].x; 
        textureInit[i * 4 + 3] = m_newNormals[i / (m_subdivY + 1)][i % (m_subdivY + 1)].y;
    }

    glBindTexture(GL_TEXTURE_2D, m_heightMapTextureRead);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_subdivX + 1, m_subdivY + 1, 0, GL_RGBA, GL_FLOAT, &textureInit[0]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void WaterGrid::refreshHeightsBuffer() {
    std::vector<GLfloat> heightBufferData(m_vertexBufferData.size() / 3);
    for (int i = 0; i < m_subdivX; i++) {
        for (int j = 0; j < m_subdivY; j++) {
            heightBufferData[i * m_subdivY * 4 + j * 4] = m_heights[i][j];
            heightBufferData[i * m_subdivY * 4 + j * 4 + 1] = m_heights[i+1][j];
            heightBufferData[i * m_subdivY * 4 + j * 4 + 2] = m_heights[i+1][j+1];
            heightBufferData[i * m_subdivY * 4 + j * 4 + 3] = m_heights[i][j+1];

            m_vertexNormalBufferData[i * m_subdivY * 12 + j * 12] = m_newNormals[i][j].x;
            m_vertexNormalBufferData[i * m_subdivY * 12 + j * 12 + 1] = sqrt(1.0f - glm::dot(m_newNormals[i][j], m_newNormals[i][j]));
            m_vertexNormalBufferData[i * m_subdivY * 12 + j * 12 + 2] = m_newNormals[i][j].y;

            m_vertexNormalBufferData[i * m_subdivY * 12 + j * 12 + 3] = m_newNormals[i+1][j].x;
            m_vertexNormalBufferData[i * m_subdivY * 12 + j * 12 + 4] = sqrt(1.0f - glm::dot(m_newNormals[i+1][j], m_newNormals[i+1][j]));
            m_vertexNormalBufferData[i * m_subdivY * 12 + j * 12 + 5] = m_newNormals[i+1][j].y;

            m_vertexNormalBufferData[i * m_subdivY * 12 + j * 12 + 6] = m_newNormals[i+1][j+1].x;
            m_vertexNormalBufferData[i * m_subdivY * 12 + j * 12 + 7] = sqrt(1.0f - glm::dot(m_newNormals[i+1][j+1], m_newNormals[i + 1][j + 1]));
            m_vertexNormalBufferData[i * m_subdivY * 12 + j * 12 + 8] = m_newNormals[i + 1][j + 1].y;

            m_vertexNormalBufferData[i * m_subdivY * 12 + j * 12 + 9] = m_newNormals[i][j + 1].x;
            m_vertexNormalBufferData[i * m_subdivY * 12 + j * 12 + 10] = sqrt(1.0f - glm::dot(m_newNormals[i][j + 1], m_newNormals[i][j + 1]));
            m_vertexNormalBufferData[i * m_subdivY * 12 + j * 12 + 11] = m_newNormals[i][j + 1].y;
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
    m_vertexBufferData.resize(m_subdivX * m_subdivY * 4 * 3);
    m_vertexNormalBufferData.resize(m_subdivX * m_subdivY * 3 * 4);
    float xOffset = m_width / m_subdivX;
    float yOffset = m_height / m_subdivY;
    float xStart = m_position.x - m_width / 2.0f;
    float yStart = m_position.y - m_height / 2.0f;

    for (int i = 0; i < m_subdivX; i++) {
        for (int j = 0; j < m_subdivY; j++) {
            m_vertexBufferData[i*m_subdivY * 12 + 12 * j] = xStart + i*xOffset;
            m_vertexBufferData[i*m_subdivY * 12 + 12 * j + 1] = yStart + j*yOffset;
            m_vertexBufferData[i*m_subdivY * 12 + 12 * j + 2] = m_position.z;

            m_vertexBufferData[i*m_subdivY * 12 + 12 * j + 3] = xStart + (i+1)*xOffset;
            m_vertexBufferData[i*m_subdivY * 12 + 12 * j + 4] = yStart + j*yOffset;
            m_vertexBufferData[i*m_subdivY * 12 + 12 * j + 5] = m_position.z;

            m_vertexBufferData[i*m_subdivY * 12 + 12 * j + 6] = xStart + (i+1)*xOffset;
            m_vertexBufferData[i*m_subdivY * 12 + 12 * j + 7] = yStart + (j+1)*yOffset;
            m_vertexBufferData[i*m_subdivY * 12 + 12 * j + 8] = m_position.z;

            m_vertexBufferData[i*m_subdivY * 12 + 12 * j + 9] = xStart + i*xOffset;
            m_vertexBufferData[i*m_subdivY * 12 + 12 * j + 10] = yStart + (j+1)*yOffset;
            m_vertexBufferData[i*m_subdivY * 12 + 12 * j + 11] = m_position.z;
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

    //Uncomment to test on shaders
#if GPU
    computeNextStep();
#else
    //Comment those 2 to disable CPU computation
    computeNextStepCPU();
    refreshTexture();
#endif
    // Handle the MVP matrix
    Renderable::prepareDrawing(camera);
    
    //refreshHeightsBuffer();
    
    // Get a handle for our "incomingRay" uniform
    GLuint handle = glGetUniformLocation(m_shader.getProg(), "eye");
    glm::vec3 eye = camera.getPosition();
    glUniform3fv(handle, 1, &eye[0]);

    handle = glGetUniformLocation(m_shader.getProg(), "halfEdgeLength");
    glUniform1f(handle, m_cube->getEdgeLength()/2.0f);

    GLuint handleCubeTexture = glGetUniformLocation(m_shader.getProg(), "cubeTexture");
    GLuint handleHeightMapTexture = glGetUniformLocation(m_shader.getProg(), "heightMap");
    glUniform1i(handleCubeTexture, 0);
    glUniform1i(handleHeightMapTexture, 2);

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, m_cube->getTexture());

    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, m_heightMapTextureRead);

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

        glDrawArrays(GL_QUADS, 0, m_vertexBufferData.size());

        glDisableVertexAttribArray(0);

    glBindVertexArray(0);

}

void WaterGrid::computeNextStep() {

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_subdivX, m_subdivY);

        m_heightMapShader.activer();

        GLuint handle = glGetUniformLocation(m_heightMapShader.getProg(), "halfEdgeLength");
        glUniform1f(handle, m_cube->getEdgeLength() / 2.0f);

        handle = glGetUniformLocation(m_heightMapShader.getProg(), "dx");
        glUniform1f(handle, 1.0f / m_subdivX);

        handle = glGetUniformLocation(m_heightMapShader.getProg(), "dy");
        glUniform1f(handle, 1.0f / m_subdivY);

        GLuint handleHeightMapTexture = glGetUniformLocation(m_heightMapShader.getProg(), "heightMap");
        glUniform1i(handleHeightMapTexture, 0);

        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, m_heightMapTextureRead);

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

        glDrawArrays(GL_QUADS, 0, m_vertexBufferData.size());

        glDisableVertexAttribArray(0);

        glBindVertexArray(0);

        // Swap textures
        glBindTexture(GL_TEXTURE_2D, m_heightMapTextureRead);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_subdivX + 1, m_subdivY + 1);
        glBindTexture(GL_TEXTURE_2D, 0);
        //GLuint save = m_heightMapTextureRead;
        //m_heightMapTextureRead = m_heightMapTextureWrite;
        //m_heightMapTextureWrite = save;
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_heightMapTextureWrite, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1280, 720);
}

void WaterGrid::computeNextStepCPU() {
    for (int i = 1; i < m_subdivX; i++) {
        for (int j = 1; j < m_subdivY; j++) {
            // Heights
            float f = 0.25f * (m_heights[i + 1][j] + m_heights[i - 1][j] + m_heights[i][j + 1] + m_heights[i][j - 1]);
            m_velocities[i][j] += (f - m_heights[i][j]) * 2.0f;
            m_velocities[i][j] *= 0.995;
            m_newHeights[i][j] = m_heights[i][j] + m_velocities[i][j];

            // Normals
            glm::vec3 dx{ 0.0f , m_width / m_subdivX , m_newHeights[i + 1][j] - m_newHeights[i][j] };
            glm::vec3 dy{ m_height / m_subdivY , 0.0f,  m_newHeights[i][j] - m_newHeights[i][j + 1] };
            glm::vec3 aux = glm::normalize(glm::cross(dy, dx));
            if(abs(aux.x) > 0.5f)
                m_newNormals[i][j].x = aux.x;
            else
                m_newNormals[i][j].x = 0.0f;
            if (abs(aux.z) > 0.5f)
                m_newNormals[i][j].y = aux.z;
            else
                m_newNormals[i][j].y = 0.0f;
        }
    }
    m_heights = m_newHeights;
}

void WaterGrid::addPerturbation() {
#if GPU
    addPerturbationGPU();
#else
    addPerturbationCPU();
#endif
}

void WaterGrid::addPerturbationCPU() {
    m_heights[m_subdivX / 2][m_subdivY / 2] = -0.2f;
}

void WaterGrid::addPerturbationGPU() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_subdivX, m_subdivY);

        m_perturbationShader.activer();

        GLuint handle = glGetUniformLocation(m_perturbationShader.getProg(), "halfEdgeLength");
        glUniform1f(handle, m_cube->getEdgeLength() / 2.0f);

        handle = glGetUniformLocation(m_perturbationShader.getProg(), "perturbationPoint");
        glUniform3fv(handle, 1, &m_position[0]);

        handle = glGetUniformLocation(m_perturbationShader.getProg(), "perturbationValue");
        glUniform1f(handle, 1.0f);

        GLuint handleHeightMapTexture = glGetUniformLocation(m_perturbationShader.getProg(), "heightMap");
        glUniform1i(handleHeightMapTexture, 0);

        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, m_heightMapTextureRead);

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

        glDrawArrays(GL_QUADS, 0, m_vertexBufferData.size());

        glDisableVertexAttribArray(0);

        glBindVertexArray(0);

        // Swap textures
        glBindTexture(GL_TEXTURE_2D, m_heightMapTextureRead);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_subdivX + 1, m_subdivY + 1);
        glBindTexture(GL_TEXTURE_2D, 0);
        //GLuint save = m_heightMapTextureRead;
        //m_heightMapTextureRead = m_heightMapTextureWrite;
        //m_heightMapTextureWrite = save;
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_heightMapTextureWrite, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, 1280, 720);
}

