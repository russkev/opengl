#version 440 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpace_matrix;
uniform mat4 model;

void main()
{
//	gl_Position = lightSpace_matrix * vec4(aPos, 1.0);
	gl_Position = lightSpace_matrix * model * vec4(aPos, 1.0);
}