#version 400 core

out vec4 color;
vec4 fragColor;
in vec2 fragTex;
in vec3 normal;

uniform sampler2D Texture;
uniform vec3 eye;

void main(){
  color = texture(Texture, fragTex);
}