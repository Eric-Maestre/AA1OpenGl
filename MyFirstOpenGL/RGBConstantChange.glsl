#version 440 core

out vec4 fragColor;

uniform float time;

void main()
{

	float t = mod(time, 6.0);

	vec3 color;
	if (t < 2.0) {
        color = vec3(1.0, 0.0, 0.0);
    } else if (t < 4.0) {
        color = vec3(0.0, 1.0, 0.0);
    } else {
        color = vec3(0.0, 0.0, 1.0);
    }
	fragColor = vec4(color, 1.0);
}