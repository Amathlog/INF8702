#version 400 core

out vec4 color;
vec4 fragColor;
in vec2 fragTex;
in vec3 normal;

uniform sampler2D Texture;
uniform vec3 eye;

void main(){
    if(dot(eye, normal) < 0){
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        fragColor = vec4(1.0);
    }
  color = texture(Texture, fragTex) * fragColor;
}