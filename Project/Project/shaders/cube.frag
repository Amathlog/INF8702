#version 400 core

out vec4 color;

in vec2 fragTex;
in vec3 normal;
in float z;

vec4 fragColor;

uniform sampler2D Texture;
uniform vec3 eye;
uniform float heightMax;

void main(){
    if(z < heightMax){
        fragColor = vec4(0.4, 0.9, 1.0, 1.0);
    } else {
        fragColor = vec4(1.0);
    }
  color = texture(Texture, fragTex) * fragColor;
}