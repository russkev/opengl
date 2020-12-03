#version 440 core
#pragma optionNV unroll all

// // ----- INS ----- // //
in vec2 uv;

// // ----- UNIFORMS ----- // //
uniform sampler2D color;
uniform sampler2D bright;

// // ----- OUTS ----- // //
out layout (location = 0) vec4 frag_color;

// // ----- MAIN ----- // //
void main()
{
	vec3 main_color = texture(color, uv).rgb;
	vec3 main_brightness = texture(bright, uv).rgb;
	frag_color = vec4(main_color + main_brightness, 1.0);
}
	