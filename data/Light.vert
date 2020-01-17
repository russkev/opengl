#version 440 core

// // ----- INS ----- // //
in layout(location = 0 ) vec3 vertex_position;

// // ----- UNIFORMS ----- // //
struct Transform
{
	mat4 model_to_projection;
};
uniform Transform transform;

// // ----- MAIN ----- // //
void main()
{
	gl_Position = transform.model_to_projection * vec4(vertex_position, 1.0);
}