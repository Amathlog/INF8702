#version 400 core

layout(location = 0) in vec3 vp;

uniform sampler2D cubeTexture;
uniform sampler2D heightMap;

uniform mat4 MVP;

uniform float halfEdgeLength;

out vec3 position;
out vec3 normal;
out float h;
out vec4 fragColor;

void main(){
    vec2 texCoords = (vp.xy + halfEdgeLength) / (2.0 * halfEdgeLength);
    vec4 textureInfo = texture2D(heightMap, texCoords);
    fragColor = textureInfo;
    h = textureInfo.r;
    position = vp;
    normal = vec3(textureInfo.b, sqrt(1.0 - dot(textureInfo.ba, textureInfo.ba)), textureInfo.a);
    vec3 newPosition = vec3(vp.xy, vp.z);
    gl_Position = MVP * vec4(newPosition, 1.0);
}

