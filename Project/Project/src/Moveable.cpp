#include "Moveable.h"
#include "glm/gtx/transform.hpp"

Moveable::Moveable(glm::vec3 position) : m_position(position){
    // Init the transform matrix to identity and position to origin
    m_transform = glm::mat4(1.0f);
}

void Moveable::setPosition(glm::vec3& position) {
    m_position = position;
}

glm::vec3 Moveable::getPosition() {
    return m_position;
}

glm::mat4 Moveable::getTransform() {
    return m_transform;
}

void Moveable::translate(glm::vec3& v) {
    m_transform = glm::translate(v) * m_transform;
}

void Moveable::scale(glm::vec3& s) {
    m_transform = glm::scale(s) * m_transform;
}

void Moveable::rotate(float angle, glm::vec3 r) {
    m_transform = glm::rotate(angle, r) * m_transform;
}
