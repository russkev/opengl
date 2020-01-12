#version 440 core
#pragma optionNV unroll all

// // ----- INS ----- // //
in vec2 uv;

// // ----- UNIFORMS ----- // //
uniform sampler2D hdr_buffer;
uniform bool is_hdr;
uniform float exposure;


// // ----- OUTS ----- // //
out vec4 fragColor;

// // ----- MAIN ----- // //
void main()
{
	const float gamma = 2.2;
	vec3 hdr_color = texture(hdr_buffer, uv).rgb;

	fragColor = vec4(hdr_color, 1.0);
}
