#version 400 core

out vec3 color;
in vec3 fragColor;

void main(){
  color = fragColor;
}