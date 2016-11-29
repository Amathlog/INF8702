#version 400 core

uniform sampler2D heightMap;
uniform float halfEdgeLength;
uniform float dx;
uniform float dy;

in vec2 texCoords;

out vec4 heightMapColor;

void main(){
    float DX = dx / (2.0 * halfEdgeLength);
    float DY = dy / (2.0 * halfEdgeLength);
    float f = 0.25 * (texture2D(heightMap, texCoords + vec2(1.0,0.0) * DX).r +
                      texture2D(heightMap, texCoords + vec2(-1.0,0.0) * DX).r +
                      texture2D(heightMap, texCoords + vec2(0.0,1.0) * DY).r +
                      texture2D(heightMap, texCoords + vec2(0.0,-1.0) * DY).r);

    heightMapColor = texture2D(heightMap, texCoords);

    heightMapColor.g += (f - heightMapColor.r) * 2.0;
    heightMapColor.g *= 0.995;
    heightMapColor.r += heightMapColor.g;

    // Normals
    vec3 vecDx = vec3(0.0, DX, texture2D(heightMap, texCoords + vec2(1.0,0.0) * DX).r - texture2D(heightMap, texCoords).r);
    vec3 vecDy = vec3(DY, 0.0, texture2D(heightMap, texCoords) - texture2D(heightMap, texCoords + vec2(0.0,1.0) * DY).r);
    vec3 aux = normalize(cross(vecDy, vecDx));
    heightMapColor.ba = aux.xz;
}