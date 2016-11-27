#pragma once

#ifndef _WATERGRID_H_
#define _WATERGRID_H_

#include <vector>
#include <GL/glew.h>
#include "Renderable.h"
#include "glm\vec3.hpp"

class WaterGrid : public Renderable{
public:
    WaterGrid(CNuanceurProg shader, glm::vec3 position, int subdivX, int subdivY, float height, float width);
    void draw(Camera& camera);

private:
    void generateGrid();
    void init();
    void perturbation();
    void computeNextStep();

    std::vector<GLfloat> m_vertexBufferData;

    std::vector<GLfloat> m_vertexNormalBufferData;

    GLuint m_vertexArrayID;
    GLuint m_vertexBuffer;
    GLuint m_vertexNormalBuffer;

    int m_subdivX;
    int m_subdivY;

    float m_height;
    float m_width;

    float m_t = 0.0f;
    int m_period = 20;
    float m_pertubation = 0.2f;
    float m_attenuation = 4.0f;
    float m_waveLength = 0.2f;
    glm::vec3 m_perturbationPoint;

    std::vector<std::vector<float>> m_velocities;
    std::vector<std::vector<float>> m_heights;
    std::vector<std::vector<float>> m_newHeights;

    float m_velocity = 0.5f;
    float m_dt = 0.1f;

};

#endif