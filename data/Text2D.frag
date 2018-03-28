#version 440 core

in vec2 uv;
layout(location = 0) uniform sampler2D	fontTexture;

out vec4 color;

void main()
{
	color = texture2D(fontTexture, uv);
}