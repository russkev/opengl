#version 440 core

// // ----- INS ----- // //
in vec2 uv;

// // ----- UNIFORMS ----- // //
uniform sampler2D base;
uniform sampler2D layer_1;

// // ----- OUTS ----- // //
out layout (location = 0) vec4 frag_color;

// // ----- MAIN ----- // //
void main()
{
	vec3 base_value = texture(base, uv).rgb;
	vec3 layer_1_value = texture(layer_1, uv).rgb;
	frag_color = vec4(base_value * layer_1_value, 1.0);
}