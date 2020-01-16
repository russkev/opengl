#version 440 core
#pragma optionNV unroll all

// // ----- INS ----- // //
in vec2 uv;

// // ----- UNIFORMS ----- // //
uniform sampler2D hdr_buffer;
uniform bool is_hdr;
uniform float exposure;


// // ----- OUTS ----- // //
out vec4 frag_color;

// // ----- MAIN ----- // //
void main()
{
	const float gamma = 2.2;
	vec3 hdr_color = texture(hdr_buffer, uv).rgb;

	//frag_color = vec4(hdr_color, 1.0) * exposure;
	//frag_color = vec4(1.0, 0.5, 1.0, 1.0);
    frag_color = texture(hdr_buffer, uv);
//	frag_color = vec4(hdr_color * 0.5, 1.0);
//	frag_color = vec4(hdr_color / (hdr_color + vec3(1.0)), 1.0);
}
