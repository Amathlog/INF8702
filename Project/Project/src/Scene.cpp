#pragma once

#include "Scene.h"

void Scene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

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

Camera& Scene::getCamera() {
    return m_camera;
}