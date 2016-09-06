#version 430 core

in vec4 fragColor;
out vec4 color;

void main () {
    
    color = fragColor;

	// Clamp the final light color.
	color = clamp(color, 0.0f, 1.0f);
}