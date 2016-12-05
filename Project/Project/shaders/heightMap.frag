#version 400 core

layout(location = 0) out vec4 color;

uniform sampler2D heightMap;
uniform float halfEdgeLength;
uniform float dx;
uniform float dy;
uniform float dt;
uniform float c;

in vec2 texCoords;
in vec3 pos;

vec4 heightMapColor;

// Vecteurs delta
vec2 deltaXp = vec2(dx, 0.0);
vec2 deltaYp = vec2(0.0, dy);
vec2 deltaXm = vec2(-dx, 0.0);
vec2 deltaYm = vec2(0.0, -dy);

void main(){

    // Edge issues, mirror if outside the texture
    if(texCoords.x >= 1.0 - dx)
        deltaXp = deltaXm;
    if(texCoords.x <= dx)
        deltaXm = deltaXp;
    if(texCoords.y >= 1.0 - dy)
        deltaYp = deltaYm;
    if(texCoords.y <= dy)
        deltaYm = deltaYp;

    float dt = 1.0;
    float c = 0.5;
    heightMapColor = texture2D(heightMap, texCoords);
    float f = (texture2D(heightMap, texCoords + deltaXp).r +
                      texture2D(heightMap, texCoords + deltaXm).r +
                      texture2D(heightMap, texCoords + deltaYp).r +
                      texture2D(heightMap, texCoords + deltaYm).r - 4 * heightMapColor.r) * c;

    

    heightMapColor.g += f * dt;
    heightMapColor.g *= 0.995;
    heightMapColor.r += heightMapColor.g * dt;

    // Normals
    vec3 vecDx = vec3(0.0, dx, texture2D(heightMap, texCoords + vec2(dx,0.0)).r - texture2D(heightMap, texCoords).r);
    vec3 vecDy = vec3(dy, 0.0, texture2D(heightMap, texCoords).r - texture2D(heightMap, texCoords + vec2(0.0,dy)).r);
    vec3 aux = normalize(cross(vecDy, vecDx));

    if(abs(aux.x)> 0.5){
      heightMapColor.b = aux.x;  
    } else {
      heightMapColor.b = 0;
    }

    if(abs(aux.z)> 0.5){
      heightMapColor.a = aux.z;  
    } else {
      heightMapColor.a = 0;
    } 

    float tmp = f;
    color = heightMapColor;
}
