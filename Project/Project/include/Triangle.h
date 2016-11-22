#pragma once

#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <GL/glew.h>
#include "Renderable.h"

class Triangle : public Renderable {

public:
    Triangle(CNuanceurProg shader);
    void draw(Camera& camera);

private:
    void init();

    // An array of 3 vectors which represents 3 vertices
    const GLfloat m_vertexBufferData[9] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };

    const GLfloat m_vertexColorBufferData[9] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f,  0.0f, 1.0f,
    };

    GLuint m_vertexArrayID;
    GLuint m_vertexBuffer;
    GLuint m_vertexColorBuffer;
};

#endif