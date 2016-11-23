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

    void generateVertex();

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
        0,1,4,
        1,5,4,
        1,2,5,
        2,6,5,
        2,3,6,
        3,7,6,
        3,0,7,
        0,4,7,
        0,1,3,
        1,2,3,
        4,5,7,
        5,6,7
    };

    std::vector<GLfloat> m_vertexColorBufferData;

    GLuint m_vertexArrayID;
    GLuint m_vertexBuffer;
    GLuint m_vertexColorBuffer;
    GLuint m_vertexIndiceBuffer;

    float m_edgeLength;
};

#endif