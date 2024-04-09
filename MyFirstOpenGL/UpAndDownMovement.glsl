#version 440 core

layout(location = 0) in vec3 posicion;

uniform mat4 time;
uniform float amplitude;
uniform float frequency;

void main()
{
    //Calcular desplazamiento vertical
    float yOffset = amplitude * sin(time * frequency);

    if(posicion.y + yOffset > 1.0 || posicion.y + yOffset < -1.0)
    {
        yOffset *= -1.0;
    }

    vec3 newPos = posicion;
    newPos.y += yOffset;

    gl_Position = vec4(newPos, 1.0);

}