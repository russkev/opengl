#version 440 core
#define NUM_LIGHTS 3

// // ----- INS ----- // //
layout (location = 0) in vec3 vertex_position;

// // ----- UNIFORMS ----- // //
struct Transform
{
	mat4 model_to_projection;
};
uniform Transform transform;

// // ----- MAIN ----- // //
void main()
{
	gl_Position = 
		transform.model_to_projection * 
		vec4(vertex_position, 1.0);
}