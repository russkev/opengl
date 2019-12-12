#version 440 core


out vec4 fragColor;


struct Light
{
	vec3 position;
	float brightness;
	vec3 color;
};
uniform Light light = { vec3(0.0), 10.0, vec3(1.0) };

void main ()
{
	fragColor = vec4(light.brightness * light.brightness * light.color, 1.0);
}
