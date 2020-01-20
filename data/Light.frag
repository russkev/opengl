#version 440 core


// // ----- OUTS ----- // //
out layout(location = 0) vec4 frag_color;

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
	frag_color = vec4(light.brightness * light.brightness * light.color, 1.0);
}
