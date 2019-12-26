#version 440 core
#define NUM_LIGHTS 3
layout (location = 0) in vec3 aPos;

struct Transform
{
	mat4 modelToWorld;
};
uniform Transform transform;

uniform mat4 projection;

void main()
{
	gl_Position = 
		projection *
		transform.modelToWorld * 
		vec4(aPos, 1.0);
}