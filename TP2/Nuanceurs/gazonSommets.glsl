#version 430 core

struct Light
{
    vec3 Ambient; 
    vec3 Diffuse;
    vec3 Specular;
    vec4 Position;  // Si .w = 1.0 -> Direction de lumiere directionelle.
    vec3 SpotDir;
    float SpotExp;
    float SpotCutoff;
    vec3 Attenuation; //Constante, Lineraire, Quadratique
};

struct Mat
{
    vec4 Ambient; 
    vec4 Diffuse;
    vec4 Specular;
    vec4 Exponent;
    float Shininess;
};

layout(location = 0) in vec2 vt;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec3 vp;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 MV_N;
uniform mat4 M;
uniform mat4 V;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoord;

void main (void)
{
    // Transformation de la position
    gl_Position = MVP * vec4(vp,1.0);
    vec4 csPosition = MV * vec4 (vp, 1.0);
	fragPos = (csPosition.xyz);
    
    //Normale en référentiel caméra:
    fragNormal = normalize(MV_N * vn);
    
    //Coordonée de texture:
    fragTexCoord = vt;
}
