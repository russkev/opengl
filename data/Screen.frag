#version 440 core
#pragma optionNV unroll all

// // ----- INS ----- // //
in vec2 uv;

// // ----- UNIFORMS ----- // //
uniform sampler2D screen_texture;

// // ----- OUTS ----- // //
out vec4 frag_color;

void main()
{
//	frag_color = vec4(1.0, 0.0, 0.0, 1.0);
	frag_color = texture(screen_texture, uv);
}