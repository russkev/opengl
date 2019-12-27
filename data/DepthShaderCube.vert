#version 440 core

layout (location = 0) in vec3 aPos;

struct Transform
{
	mat4 modelToWorld;
};
uniform Transform transform;

void main()
{
	gl_Position = transform.modelToWorld * vec4(aPos, 1.0);
}