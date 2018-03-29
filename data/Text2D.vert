#version 440 core

in layout(location = 0) vec2 model_vertexPosition;
in layout(location = 1) vec2 model_uv;

uniform int width;
uniform int height;

int half_width	= width / 2;
int half_height = height / 2;

out vec2 uv;


void main()
{
	vec2 outPosition	= model_vertexPosition - vec2(half_width, half_height);
	outPosition			/= vec2(half_width, half_height);

	gl_Position			= vec4(outPosition, 0, 1);
	uv					= model_uv;
}