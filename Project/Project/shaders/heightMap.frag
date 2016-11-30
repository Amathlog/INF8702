#version 400 core

layout(location = 0) out vec4 color;

uniform sampler2D heightMap;
uniform float halfEdgeLength;
uniform float dx;
uniform float dy;

in vec2 texCoords;

vec4 heightMapColor;

void main(){
    float f = 0.25 * (texture2D(heightMap, texCoords + vec2(dx,0.0)).r +
                      texture2D(heightMap, texCoords - vec2(dx,0.0)).r +
                      texture2D(heightMap, texCoords + vec2(0.0,dy)).r +
                      texture2D(heightMap, texCoords - vec2(0.0,dy)).r);

    heightMapColor = texture2D(heightMap, texCoords);

    heightMapColor.g += (f - heightMapColor.r) * 2.0;
    heightMapColor.g *= 0.5;
    heightMapColor.r += heightMapColor.g;

    // Normals
    vec3 vecDx = vec3(0.0, dx, texture2D(heightMap, texCoords + vec2(dx,0.0)).r - texture2D(heightMap, texCoords).r);
    vec3 vecDy = vec3(dy, 0.0, texture2D(heightMap, texCoords) - texture2D(heightMap, texCoords + vec2(0.0,dy)).r);
    vec3 aux = normalize(cross(vecDy, vecDx));
    heightMapColor.ba = aux.xz;

    color = heightMapColor;
}
