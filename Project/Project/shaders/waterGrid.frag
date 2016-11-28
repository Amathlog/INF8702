#version 400 core

uniform sampler2D Texture;

out vec4 color;
in vec3 fragColor;
in vec2 texCoord;

void main(){
    //color = vec4(fragColor, 1.0);
    //color = vec4(0.0, 0.0, 0.0, 0.0);
    color = texture(Texture, texCoord)  * vec4(0.4, 0.9, 1.0, 1.0);

    //color = vec4(texCoord, 0.0,1.0);
}