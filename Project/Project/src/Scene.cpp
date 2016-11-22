#pragma once

#include "Scene.h"

void Scene::render() {
    for (auto r : m_renderables) {
        r->draw(m_camera);
    }
}

void Scene::addRenderable(Renderable* renderable) {
    m_renderables.push_back(renderable);
}

void Scene::setCamera(Camera& camera) {
    m_camera = camera;
}