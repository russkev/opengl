#version 440 core

in vec2 uv;
in vec3 vertex_color;
//layout(location = 0) uniform sampler2D	fontTexture;
uniform sampler2D	fontTexture;


out vec4 color;

void main()
{
	color = vec4(vertex_color, texture2D(fontTexture, uv).a);
}