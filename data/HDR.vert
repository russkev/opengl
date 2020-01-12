#version 440 core
#pragma optionNV unroll all

// // ----- INS ----- // //
in layout (location = 0) vec3 vertex_position;
in layout(location = 3 ) vec2 vertex_uv;

// // ----- OUTS ----- // //
out vec2 uv;

// // ----- MAIN ----- // //
void main()
{
	uv = vertex_uv;
	gl_Position = vec4(vertex_position, 1.0);
}
