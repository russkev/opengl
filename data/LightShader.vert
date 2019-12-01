#version 440 core

in layout(location = 0 ) vec3 model_vertexPosition;
//in layout(location = 7 ) mat4 mat_modelToProjection;

uniform mat4 mat_modelToProjection;

void main()
{
	gl_Position = mat_modelToProjection * vec4(model_vertexPosition, 1.0);
}
