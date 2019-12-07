#version 440 core

out vec4 fragColor;

uniform vec4 uColor;
uniform vec3 light_position;

in vec3 worldSpace_vertexPosition;
in vec3 worldSpace_vertexNormal;


vec3 diffuse_lighting()
{
	float cos_theta = clamp(dot(worldSpace_vertexNormal, light_position), 0, 1);
	return vec3(cos_theta, cos_theta, cos_theta);
}

void main ()
{
	fragColor =  vec4(diffuse_lighting(), 1.0) * uColor;
	//fragColor = vec4(0.3, 1.0, 0.8, 1.0);
}
