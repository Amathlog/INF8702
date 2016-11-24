#pragma once

#ifndef _LINE_H_
#define _LINE_H_

#include <GL/glew.h>
#include "Renderable.h"
#include "glm\vec3.hpp"

class Line : public Renderable {

public:
    Line(CNuanceurProg shader, glm::vec3 direction, glm::vec3 position = glm::vec3(0.0f), glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f), float length = 1.0f);
    void draw(Camera& camera);

private:
    void init();

    GLuint m_vertexArrayID;
    GLuint m_vertexBuffer;
    GLuint m_vertexColorBuffer;
    float m_length;
    glm::vec3 m_direction;
    glm::vec3 m_color;
};

#endif