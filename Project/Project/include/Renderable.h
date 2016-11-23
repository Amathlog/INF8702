#pragma once

#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include "NuanceurProg.h"
#include "Camera.h"
#include "glm\vec3.hpp"

class Renderable : public Moveable{
public:
    Renderable(CNuanceurProg shader, glm::vec3 position = glm::vec3(0.0f)) : Moveable(position), m_shader(shader){}
    virtual void draw(Camera& camera) = 0;

protected:
    void prepareDrawing(Camera& camera);

    CNuanceurProg m_shader;
};

#endif