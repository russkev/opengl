#version 440 core
#pragma optionNV unroll all

// // ----- INS ----- // //
in vec2 uv;

// // ----- UNIFORMS ----- // //
uniform sampler2D color;
uniform sampler2D threshold;

// // ----- OUTS ----- // //
out layout (location = 0) vec4 frag_color;

// // ----- MAIN ----- // //
void main()
{
	vec3 main_color = texture(color, uv).rgb;
	vec3 main_brightness = texture(threshold, uv).rgb;
//	frag_color = vec4(main_color * 0.1 + main_brightness, 1.0);
	frag_color = vec4(main_brightness + main_color, 1.0);
}
	