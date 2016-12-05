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
    // Grid
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

        glGenBuffers(1, &m_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, m_vertexBufferData.size() * sizeof(GLfloat), &m_vertexBufferData[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Quad for texture
    std::vector<GLfloat> quadData = { -1.0, -1.0, m_position.z,
                                     1.0, -1.0, m_position.z,
                                     1.0, 1.0, m_position.z,
                                     -1.0, 1.0, m_position.z };

    glGenVertexArrays(1, &m_vertexArrayTextureID);
    glBindVertexArray(m_vertexArrayTextureID);

        glGenBuffers(1, &m_vertexTextureBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexTextureBuffer);
            glBufferData(GL_ARRAY_BUFFER, quadData.size() * sizeof(GLfloat), &quadData[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindVertexArray(0);

    // Framebuffer for texture update
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);


    std::vector<GLfloat> textureInit(m_subdivX * m_subdivY * 4);
    for (int i = 0; i < m_subdivX * m_subdivY; i++) {
        textureInit[i * 4] = 0.0f; // Height = 0
        textureInit[i * 4 + 1] = 0.0f; // Velocity = 0
        textureInit[i * 4 + 2] = 0.0f; // Normal x = 0
        textureInit[i * 4 + 3] = 1.0f; // Normal z = 1
    }

        glGenTextures(1, &m_heightMapTextureRead);
        glBindTexture(GL_TEXTURE_2D, m_heightMapTextureRead);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_subdivX, m_subdivY, 0, GL_RGBA, GL_FLOAT, &textureInit[0]);           

        glBindTexture(GL_TEXTURE_2D, 0);

        glGenTextures(1, &m_heightMapTextureWrite);
            glBindTexture(GL_TEXTURE_2D, m_heightMapTextureWrite);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_subdivX, m_subdivY, 0, GL_RGBA, GL_FLOAT, &textureInit[0]);

        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_heightMapTextureWrite, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR FRAMEBUFFER" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
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
}


void WaterGrid::draw(Camera& camera) {

    //Double for the speed
    computeNextStep();
    computeNextStep();

    // Handle the MVP matrix
    Renderable::prepareDrawing(camera);

    // Print the center of the grid in window space
    glm::vec4 p = camera.getProjectionMatrix() * camera.getViewMatrix() * glm::vec4(m_position, 1.0f);
    //std::cout << "After projection : (" << p.x << ", " << p.y << ", " << p.z << ", " << p.w << ")" << std::endl;
 
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
    // Activate the shader
    m_heightMapShader.activer();

    // Push specific uniform variables to the shader
    GLuint handle = glGetUniformLocation(m_heightMapShader.getProg(), "dx");
    glUniform1f(handle, 1.0f / m_subdivX);

    handle = glGetUniformLocation(m_heightMapShader.getProg(), "dy");
    glUniform1f(handle, 1.0f / m_subdivY);

    handle = glGetUniformLocation(m_heightMapShader.getProg(), "dt");
    glUniform1f(handle, m_deltaT);

    handle = glGetUniformLocation(m_heightMapShader.getProg(), "c");
    glUniform1f(handle, m_celerity);

    // Draw into the texture
    drawIntoTexture();
}


void WaterGrid::addPerturbation(glm::vec3 position) {
    addPerturbationGPU(position);
}

void WaterGrid::addPerturbationGPU(glm::vec3 position) {
    m_perturbationShader.activer();

    // Push specific uniform variables to the shader
    GLuint handle = glGetUniformLocation(m_perturbationShader.getProg(), "perturbationPoint");
    position.x /= (m_cube->getEdgeLength() * 0.5f);
    position.y /= (m_cube->getEdgeLength() * 0.5f);
    glUniform3fv(handle, 1, &position[0]);

    handle = glGetUniformLocation(m_perturbationShader.getProg(), "perturbationStrength");
    glUniform1f(handle, m_perturbationStrength);

    handle = glGetUniformLocation(m_perturbationShader.getProg(), "perturbationRadius");
    glUniform1f(handle, m_perturbationRadius);

    // Draw into the texture
    drawIntoTexture();

}

void WaterGrid::drawIntoTexture() {
    // Don't forget to adapt the viewport to the size of the texture
    // We save the old values before
    GLint m_viewport[4];

    glGetIntegerv(GL_VIEWPORT, m_viewport);

    glViewport(0, 0, m_subdivX, m_subdivY);
    // Bind the FBO to draw it in the texture.
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        GLuint handle = glGetUniformLocation(m_perturbationShader.getProg(), "halfEdgeLength");
        glUniform1f(handle, m_cube->getEdgeLength() / 2.0f);

        GLuint handleHeightMapTexture = glGetUniformLocation(m_heightMapShader.getProg(), "heightMap");
        glUniform1i(handleHeightMapTexture, 0);

        // Bind the texture
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, m_heightMapTextureRead);

        // Draw everything
        glBindVertexArray(m_vertexArrayTextureID);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexTextureBuffer);

        glVertexAttribPointer(
            0,                          // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                          // size
            GL_FLOAT,                   // type
            GL_FALSE,                   // normalized?
            0,                          // stride
            (void*)0);                  // array buffer offset

        glDrawArrays(GL_QUADS, 0, 4);

        glDisableVertexAttribArray(0);

        glBindVertexArray(0);

        // Swap textures
        GLuint save = m_heightMapTextureRead;
        m_heightMapTextureRead = m_heightMapTextureWrite;
        m_heightMapTextureWrite = save;
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_heightMapTextureWrite, 0);

    // Don't forget to unbind the buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Get back to the old viewport
    glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

}

