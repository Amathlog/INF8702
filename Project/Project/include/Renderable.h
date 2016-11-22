#pragma once

#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include "NuanceurProg.h"
#include "Camera.h"

class Renderable : public Moveable{
public:
    Renderable(CNuanceurProg shader) : m_shader(shader){}
    virtual void draw(Camera& camera) = 0;

protected:
    void prepareDrawing(Camera& camera);

    CNuanceurProg m_shader;
};

#endif