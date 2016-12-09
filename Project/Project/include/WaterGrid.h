#pragma once

#ifndef _WATERGRID_H_
#define _WATERGRID_H_

#include <vector>
#include <GL/glew.h>
#include "Renderable.h"
#include "glm/vec3.hpp"
#include "Cube.h"
#include "NuanceurProg.h"

class WaterGrid : public Renderable{
public:
    WaterGrid(CNuanceurProg shader, glm::vec3 position, int subdivX, int subdivY, float height, float width);
    void draw(Camera& camera);
    void addPerturbation(glm::vec3 position);

    void setCube(Cube* cube);

private:
    void generateGrid();
    void init();
    void computeNextStep();
    void addPerturbationGPU(glm::vec3 position);
    void drawIntoTexture();

    std::vector<GLfloat> m_vertexBufferData;

    std::vector<GLfloat> m_vertexNormalBufferData;

    GLuint m_vertexArrayID;
    GLuint m_vertexBuffer;

    GLuint m_vertexArrayTextureID;
    GLuint m_vertexTextureBuffer;

    int m_subdivX;
    int m_subdivY;

    float m_height;
    float m_width;

    Cube* m_cube;
    GLuint m_texture;

    GLuint m_fbo;
    GLuint m_heightMapTextureRead;
    GLuint m_heightMapTextureWrite;

    CNuanceurProg m_heightMapShader;
    CNuanceurProg m_perturbationShader;

    // Constants for simulation
    float m_perturbationRadius = 0.02f;     // Radius of the perturbation
    float m_perturbationStrength = 0.1f;    // Strength of the perturbation
    float m_deltaT = 1.0f;                  // Time step
    float m_celerity = 0.5f;                // Speed of the wave

};

#endif
