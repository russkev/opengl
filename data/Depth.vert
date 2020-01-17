#version 440 core
#define NUM_LIGHTS 3

// // ----- INS ----- // //
layout (location = 0) in vec3 aPos;

// // ----- UNIFORMS ----- // //
struct Transform
{
	mat4 model_to_world;
};
uniform Transform transform;

uniform mat4 projection;

// // ----- MAIN ----- // //
void main()
{
	gl_Position = 
		projection *
		transform.model_to_world * 
		vec4(aPos, 1.0);
}