#version 400 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in float h;

float M_PI = 3.14159265359;

uniform float t;
uniform float maxPerturbation;
uniform float attenuation;
uniform float waveLength;
uniform vec3 perturbationPoint;
uniform vec3 eye;
uniform float halfEdgeLength;

uniform mat4 MVP;

out vec3 fragColor;
out vec2 texCoord;

float computeSinusOffset(){
    float dist = distance(vec2(perturbationPoint), vec2(vp));
    return sin(t + dist / waveLength * M_PI) * maxPerturbation / pow(dist + 1, attenuation);
}

vec3 getIntersection(vec3 position, vec3 ray, vec3 planNormal, float halfEdgeLength){
    vec3 diff = planNormal * halfEdgeLength - position;
    float t = planNormal.x * diff.x / ray.x + planNormal.y * diff.y / ray.y + planNormal.z * diff.z / ray.z;
    return position + t*ray;
}

vec2 getTexCoords(vec3 position, vec3 ray, float halfEdgeLength){
    vec3 intersection = getIntersection(position, ray, vec3(0,0,1), -halfEdgeLength);
    if(abs(intersection.x) < halfEdgeLength && abs(intersection.y) < halfEdgeLength){
        return vec2((intersection.x + halfEdgeLength)/(2.0 * halfEdgeLength), (intersection.y + halfEdgeLength)/(2.0 * halfEdgeLength));
    } else if (abs(intersection.x) > halfEdgeLength && abs(intersection.y) > halfEdgeLength){
        if (abs(intersection.x) < abs(intersection.y)){
            intersection.x = 0;
        } else {
            intersection.y = 0;
        }
    }


    if(intersection.x > halfEdgeLength){
        intersection = getIntersection(position, ray, vec3(1,0,0), halfEdgeLength);
        return vec2((intersection.z + halfEdgeLength)/(2.0 * halfEdgeLength), 1.0 - (intersection.y + halfEdgeLength)/(2.0 * halfEdgeLength));
    } else if(intersection.x < -halfEdgeLength){
        intersection = getIntersection(position, ray, vec3(1,0,0), -halfEdgeLength);
        return vec2((intersection.z + halfEdgeLength)/(2.0 * halfEdgeLength), (intersection.y + halfEdgeLength)/(2.0 * halfEdgeLength));
    } else if(intersection.y > halfEdgeLength){
        intersection = getIntersection(position, ray, vec3(0,1,0), halfEdgeLength);
        return vec2((intersection.z + halfEdgeLength)/(2.0 * halfEdgeLength), (intersection.x + halfEdgeLength)/(2.0 * halfEdgeLength));
    } else if(intersection.y < -halfEdgeLength){
        intersection = getIntersection(position, ray, vec3(0,1,0), -halfEdgeLength);
        return vec2((intersection.z + halfEdgeLength)/(2.0 * halfEdgeLength), (intersection.x + halfEdgeLength)/(2.0 * halfEdgeLength));
    }
    else {
        return vec2(-1.0);
    }
}

void main(){
    float offset = computeSinusOffset();
    vec3 newPosition = vec3(vp.xy, vp.z + h);
    gl_Position = MVP * vec4(newPosition, 1.0);
    //fragColor = clamp(vec3(0.0, h/maxPerturbation, 1.0 + h/maxPerturbation), 0.0,1.0);

    vec3 incomingRay = vp - eye;
    vec3 refractedRay = refract(incomingRay, vn, 1.0/1.3330);
    //refractedRay = incomingRay;
    
    texCoord = getTexCoords(newPosition, refractedRay, halfEdgeLength);

    //fragColor = vec3((1.0+dot(vec3(0.0, 0.0, 1.0), vec3(vn.x, 0.0, vn.z)))/2.0, (1.0+dot(vec3(0.0, 0.0, 1.0), vec3(0.0, vn.y, vn.z)))/2.0, 1.0);
    //fragColor = vn;
}

