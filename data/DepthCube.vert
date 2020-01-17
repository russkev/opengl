#version 440 core

// // ----- INS ----- // //
layout (location = 0) in vec3 aPos;

// // ----- UNIFORMS ----- // //
struct Transform
{
	mat4 model_to_world;
};
uniform Transform transform;

// // ----- MAIN ----- // //
void main()
{
	gl_Position = transform.model_to_world * vec4(aPos, 1.0);
}