#version 440 core
#define NUM_LIGHTS 3
#define NUM_FACES 6

// // ----- INS ----- // //
layout (triangles) in;

// // ----- OUTS ----- // //
layout (triangle_strip, max_vertices = 18) out;

// // ----- UNIFORMS ----- // //
struct Shadow
{
	mat4 transforms;
};
uniform Shadow shadow[NUM_FACES];

// // ----- OUTS ----- // //
out vec4 frag_position;

// // ----- MAIN ----- // //
void main()
{
	for (int face = 0; face < NUM_FACES; ++face)
	{
		gl_Layer = face;
		for(int i = 0; i < 3; i++)
		{
			frag_position = gl_in[i].gl_Position;
			gl_Position = shadow[face].transforms * frag_position;
			EmitVertex();
		}
		EndPrimitive();
	}
}
