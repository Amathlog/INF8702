#version 400 core

layout(location = 0) in vec3 vp;

uniform sampler2D cubeTexture;
uniform sampler2D heightMap;

uniform mat4 MVP;

uniform float halfEdgeLength;

out vec3 position;
out vec3 normal;
out float h;

void main(){
    vec2 texCoords = (vec2(vp) + halfEdgeLength * vec2(1.0)) / (2.0*halfEdgeLength);
    h = texture2D(heightMap, texCoords).r;
    position = vp;
    normal = vn;
    vec3 newPosition = vec3(vp.xy, vp.z + h);
    gl_Position = MVP * vec4(newPosition, 1.0);
}

