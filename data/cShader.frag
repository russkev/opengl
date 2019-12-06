#version 440 core

out vec4 fragColor;

uniform vec4 uColor;

void main ()
{
	fragColor = uColor;
	//fragColor = vec4(0.3, 1.0, 0.8, 1.0);
}
