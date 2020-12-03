#version 430 core

in layout (location = 0) vec3 vertex_position;

out vec3 uv;

struct Transform
{
	mat4 model_to_projection;
};
uniform Transform transform;

void main()
{
	uv = vertex_position;
	gl_Position = transform.model_to_projection * vec4(vertex_position, 1.0);
}


