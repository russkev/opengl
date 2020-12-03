#version 440 core
#define NUM_LIGHTS 3

// // ----- INS ----- // //
layout (location = 0) in vec3 vertex_position;

// // ----- UNIFORMS ----- // //
struct Transform
{
	mat4 model_to_projection;
	mat4 model_to_world;
};
uniform Transform transform;

uniform mat4 projection;

// // ----- MAIN ----- // //
void main()
{
	gl_Position = 
		 transform.model_to_projection * 
//		projection * transform.model_to_world *
		vec4(vertex_position, 1.0);
}