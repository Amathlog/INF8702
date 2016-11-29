#version 400 core

uniform sampler2D Texture;

out vec4 color;
in vec3 fragColor;
uniform float halfEdgeLength;
//in vec2 texCoord;
in vec3 position;
in vec3 normal;
uniform vec3 eye;
in float oh;

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
        vec3 halfpoint = vec3(0,0,0.5);
        halfpoint.x = sign(intersection.x)*halfEdgeLength;
        halfpoint.y = sign(intersection.y)*halfEdgeLength;
        vec3 dir = halfpoint - eye;
        vec3 secondIntersection = getIntersection(halfpoint, dir, vec3(0,0,1), -halfEdgeLength);
        float a = (eye.y-halfpoint.y) / (eye.x-halfpoint.x );
        float b = halfpoint.y-a*halfpoint.x;
        if (abs(a*intersection.x+b) < abs(intersection.y)){
            intersection.x = 0;
        } else {
            intersection.y = 0;
        }
    }


    if(intersection.x > halfEdgeLength){
        intersection = getIntersection(position, ray, vec3(1,0,0), halfEdgeLength);
        return vec2((intersection.z + halfEdgeLength)/(2.0 * halfEdgeLength), (intersection.y + halfEdgeLength)/(2.0 * halfEdgeLength));
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
    //color = vec4(fragColor, 1.0);
    //color = vec4(0.0, 0.0, 0.0, 0.0);
    vec3 incomingRay = position - eye;
    vec3 refractedRay = refract(incomingRay, normal, 1.0/1.3330);
    vec3 newPosition = vec3(position.xy, position.z + oh);
    color = texture(Texture, getTexCoords(newPosition, refractedRay, halfEdgeLength))  * vec4(0.4, 0.9, 1.0, 1.0);

    //color = vec4(texCoord, 0.0,1.0);
}