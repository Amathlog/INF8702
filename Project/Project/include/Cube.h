#pragma once

#ifndef _CUBE_H_
#define _CUBE_H_

#include <GL/glew.h>
#include "Renderable.h"
#include "glm\vec3.hpp"
#include <vector>

class Cube : public Renderable {

public:
    Cube(CNuanceurProg shader, glm::vec3 position = glm::vec3(0.0f), float edgeLength = 2.0f);
    void draw(Camera& camera);

private:
    void init();

    void generateBuffers();

    // An array of 3 vectors which represents 3 vertices
    std::vector<GLfloat> m_vertexBufferData;


/*           3-------2
            /|      /|
           / |     / |
          0--|----1  |
          |  7----|--6
          | /     | /
          |/      |/
          4-------5
*/
    const unsigned short m_vertexIndiceData[36] = {
        0,4,1,
        1,4,5,
        1,5,2,
        2,5,6,
        2,6,3,
        3,6,7,
        3,7,0,
        0,7,4,
        0,1,3,
        1,2,3,
        4,7,5,
        5,7,6
    };

    std::vector<GLfloat> m_vertexNormalBufferData;
    std::vector<GLfloat> m_vertexTexBufferData;

    GLuint m_vertexArrayID;
    GLuint m_vertexBuffer;
    GLuint m_vertexNormalBuffer;
    GLuint m_vertexTexBuffer;
    GLuint m_vertexIndiceBuffer;

    GLuint m_texture;

    float m_edgeLength;
};

#endif