#version 440 core

layout(location = 0) in vec3 posicion;

uniform float time;
const float speed = 0.5; 

void main()
{

    float yOffset = speed * (mod(time, 2.0) - 1.0);
    

    vec3 newPos = posicion;
    newPos.y += yOffset;
    
    gl_Position = vec4(newPos, 1.0);
}