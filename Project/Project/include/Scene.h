#pragma once

#ifndef _SCENE_H_
#define _SCENE_H_

#include "Renderable.h"
#include "Camera.h"

#include <vector>

class Scene {

public:
    void render();
    void addRenderable(Renderable* renderable);
    void setCamera(Camera& m_camera);
    Camera& getCamera();

private:
    std::vector<Renderable*> m_renderables;
    Camera m_camera;
};

#endif