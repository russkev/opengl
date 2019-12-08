#version 440 core

in layout(location = 0 ) vec3 model_vertexPosition;

struct Transform
{
	mat4 modelToProjection;
};

uniform Transform transform;

void main()
{
	gl_Position = transform.modelToProjection * vec4(model_vertexPosition, 1.0);
}