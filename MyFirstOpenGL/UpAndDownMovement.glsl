#version 440 core

layout(location = 0) in vec3 posicion;

uniform mat4 translationMatrix;
uniform mat4 rotationMatrix;
uniform mat4 scaleMatrix;

uniform float time;
uniform float speed;



void main()
{

    float verticalOffset = speed * time;
    vec3 newPosition = posicion + vec3(0.0, verticalOffset, 0.0);


    mat4 matrix = translationMatrix*rotationMatrix*scaleMatrix;
    gl_Position = matrix * vec4(newPosition, 1.0);
}