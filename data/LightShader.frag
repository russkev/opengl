#version 440 core


// // ----- OUTS ----- // //
out vec4 fragColor;

// // ----- UNIFORMS ----- // //
struct Light
{
	vec3 position;
	float brightness;
	vec3 color;
};
uniform Light light = { vec3(0.0), 10.0, vec3(1.0) };

// // ----- MAIN ----- // //
void main ()
{
	fragColor = vec4(light.brightness * light.color, 1.0);
}
