#version 450 core

layout(location = 0) out vec4 heightMapColor;

uniform sampler2D heightMap;

uniform float halfEdgeLength;
uniform vec3 perturbationPoint;
uniform float perturbationStrength;
uniform float perturbationRadius;

in vec2 texCoords;
in vec3 position;

float epsilon = 0.01;

void main(){
    heightMapColor = texture2D(heightMap, texCoords);

    float drop = max(0.0, 1.0 - length(perturbationPoint.xy * 0.5 + 0.5 - texCoords) / perturbationRadius);
    drop = -0.5 + cos(drop * 3.14159265359) * 0.5;

    // Drop only if the perturbation point is on the watergrid
    if(position.z - epsilon < perturbationPoint.z && perturbationPoint.z < position.z + epsilon)
        heightMapColor.r += drop * perturbationStrength;   

}
