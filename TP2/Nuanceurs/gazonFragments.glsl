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
    vec3 Attenuation; // Constante, Lineraire, Quadratique
};

struct Mat
{
    vec4 Ambient; 
    vec4 Diffuse;
    vec4 Specular;
    vec4 Exponent;
    float Shininess;
};

uniform sampler2D colorMap;
uniform Light Lights[3]; // 0:ponctuelle  1:spot  2:directionnelle
uniform Mat Material;
uniform int pointLightOn;
uniform int spotLightOn;
uniform int dirLightOn;

in vec2 fragTexCoord;
in vec3 fragPos;
in vec3 fragNormal;
out vec4 color;

// Accumulateurs de contributions Ambiantes et Diffuses:
vec4 Ambient;
vec4 Diffuse;

// Calcul pour une lumière ponctuelle
void pointLight(in vec3 lightVect, in vec3 normal)
{
    float nDotVP;       // Produit scalaire entre VP et la normale
    float attenuation;  // facteur d'atténuation calculé
    float d;            // distance entre lumière et fragment
    vec3  VP;           // Vecteur lumière

    // Calculer vecteur lumière
    VP = normalize(lightVect);

    // Calculer distance à la lumière
    d = length(lightVect);

    // Calculer l'atténuation due à la distance
    attenuation = 1.0 / (Lights[0].Attenuation[0] + Lights[0].Attenuation[1] * d + Lights[0].Attenuation[2] * d*d);

    nDotVP = dot(VP, normal);

    // Calculer les contributions ambiantes et diffuses
    Ambient += vec4(Lights[0].Ambient * attenuation, 1.0);
    Diffuse += vec4(Lights[0].Diffuse * nDotVP * attenuation, 1.0);
}


// Calcul pour une lumière directionnelle
void directionalLight(in vec3 lightVect, in vec3 normal)
{
    vec3  VP;             // Vecteur lumière
    float nDotVP;         // Produit scalaire entre VP et la normale

    VP = normalize(-lightVect);

    nDotVP = dot(VP, normal);

    // Calculer les contributions ambiantes et diffuses
    Ambient += vec4(Lights[2].Ambient, 1.0);
    Diffuse += vec4(Lights[2].Diffuse * nDotVP, 1.0);
}


// Calcul pour une lumière "spot"
void spotLight(in vec3 lightVect, in vec3 normal)
{
    float nDotVP;             // Produit scalaire entre VP et la normale
    float spotAttenuation;    // Facteur d'atténuation du spot
    float attenuation;        // Facteur d'atténuation du à la distance
    float angleEntreLumEtSpot;// Angle entre le rayon lumieux et le milieu du cone
    float d;                  // Distance à la lumière
    vec3  VP;                 // Vecteur lumière

    // Calculer vecteur lumière
    VP = normalize(lightVect);

    // Calculer distance à la lumière
    d = length(lightVect);

    // Calculer l'atténuation due à la distance
    attenuation = 1.0 / (Lights[1].Attenuation[0] + Lights[1].Attenuation[1] * d + Lights[1].Attenuation[2] * d*d);

    // Le fragment est-il à l'intérieur du cône de lumière ?
    vec3 spotDir = normalize(Lights[1].SpotDir);
    vec3 lightDir = VP;
    angleEntreLumEtSpot = acos(dot(lightDir, -spotDir)) * 180.0 / 3.14159265359;

    if (angleEntreLumEtSpot > Lights[1].SpotCutoff){
        spotAttenuation = 0.0; // en dehors... aucune contribution
    } else {
        spotAttenuation = pow(angleEntreLumEtSpot, Lights[1].SpotExp);
    }

    // Combine les atténuation du spot et de la distance
    attenuation *= spotAttenuation;

    nDotVP = dot(VP, normal);

    // Calculer les contributions ambiantes et diffuses
    Ambient  += vec4(Lights[1].Ambient * attenuation, 1.0);
    Diffuse  += vec4(Lights[1].Diffuse * nDotVP * attenuation, 1.0);
}

vec4 flight(in vec3 light0Vect, in vec3 light1Vect, in vec3 light2Vect, in vec3 normal)
{
    vec4 color;
    vec3 ecPosition3;

    // Réinitialiser les accumulateurs
    Ambient  = vec4 (0.0);
    Diffuse  = vec4 (0.0);

    if (pointLightOn == 1) {
        pointLight(light0Vect, normal);
    }
    if (spotLightOn == 1) {
        spotLight(light1Vect, normal);
    }
    if (dirLightOn == 1) {
        directionalLight(light2Vect, normal);
    }

    color = (Ambient * Material.Ambient + Diffuse  * Material.Diffuse);
    color = clamp( color, 0.0, 1.0 );
    
    return color;
}

void main (void) 
{
    vec4 out_color;
	vec3 light0Vect;
    vec3 light1Vect;
    vec3 light2Vect;

	// Vecteurs de la surface vers la lumière
	light0Vect = Lights[0].Position.xyz - fragPos;
    light1Vect = Lights[1].Position.xyz - fragPos;
	light2Vect = Lights[2].Position.xyz;

    // Contribution des lumières
	out_color = flight(light0Vect, light1Vect, light2Vect, fragNormal);

    // Contribution de la texture:
    out_color *= texture(colorMap, fragTexCoord);
    
    color = clamp(out_color, 0.0, 1.0);
}