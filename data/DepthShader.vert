#version 440 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpace_matrix;
uniform mat4 model;

void main()
{
	gl_position = lightSpace_matrix * model * vec4(aPos, 1.0);
}