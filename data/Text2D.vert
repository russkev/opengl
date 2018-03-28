#version 440 core

in layout(location = 0) vec2 model_vertexPosition;
in layout(location = 1) vec2 model_uv;

out vec2 uv;


void main()
{
	vec2 outPosition	= model_vertexPosition - vec2(400, 300);
	outPosition			/= vec2(400, 300);

	gl_Position			= vec4(outPosition, 0, 1);
	uv					= model_uv;
}