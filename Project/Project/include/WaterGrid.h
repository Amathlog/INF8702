#pragma once

#ifndef _WATERGRID_H_
#define _WATERGRID_H_

#include <vector>
#include <GL/glew.h>
#include "Renderable.h"
#include "glm\vec3.hpp"
#include "Cube.h"

class WaterGrid : public Renderable{
public:
    WaterGrid(CNuanceurProg shader, glm::vec3 position, int subdivX, int subdivY, float height, float width);
    void draw(Camera& camera);
    void addPerturbation();

    void setCube(Cube* cube);

private:
    void generateGrid();
    void init();
    void computeNextStep();
    void refreshHeightsBuffer();

    std::vector<GLfloat> m_vertexBufferData;

    std::vector<GLfloat> m_vertexNormalBufferData;

    GLuint m_vertexArrayID;
    GLuint m_vertexBuffer;
    GLuint m_vertexNormalBuffer;
    GLuint m_vertexHeightBuffer;

    int m_subdivX;
    int m_subdivY;

    float m_height;
    float m_width;
    
    std::vector<std::vector<GLfloat>> m_velocities;
    std::vector<std::vector<GLfloat>> m_heights;
    std::vector<std::vector<GLfloat>> m_newHeights;
    std::vector<std::vector<glm::vec2>> m_newNormals;

    Cube* m_cube;
    GLuint m_texture;
};

#endif