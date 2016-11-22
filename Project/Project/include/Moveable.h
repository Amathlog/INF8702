#ifndef _MOVEABLE_H_
#define _MOVEABLE_H_

#include "glm\vec3.hpp"
#include "glm\mat4x4.hpp"

class Moveable {
public:

    Moveable();

    void setPosition(glm::vec3& position);
    glm::vec3 getPosition();
    glm::mat4 getTransform();

    /***
        Translate the object following v
        @param v = vector translation
    ***/
    void translate(glm::vec3& v);

    /***
        Scale the object following s
        @param s = vector scaling
    ***/
    void scale(glm::vec3& s);

    /***
        Rotate the object around r of angle degrees
        @param angle = angle of the rotation in degrees
        @param r = vector rotation
    ***/
    void rotate(float angle, glm::vec3 r);

protected:
    glm::mat4 m_transform;
    glm::vec3 m_position;
};

#endif