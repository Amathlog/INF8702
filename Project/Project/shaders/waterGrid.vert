#version 400 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in float h;

uniform mat4 MVP;

out vec3 position;
out vec3 normal;
out float oh;

void main(){
    position = vp;
    normal = vn;
    oh = h;
    vec3 newPosition = vec3(vp.xy, vp.z + h);
    gl_Position = MVP * vec4(newPosition, 1.0);
}

