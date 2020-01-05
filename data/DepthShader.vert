#version 440 core
#define NUM_LIGHTS 3

layout (location = 0) in vec3 aPos;

struct Transform
{
	mat4 model_to_world;
};
uniform Transform transform;

uniform mat4 projection;

void main()
{
	gl_Position = 
		projection *
		transform.model_to_world * 
		vec4(aPos, 1.0);
}