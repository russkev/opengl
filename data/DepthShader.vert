#version 440 core
#define NUM_LIGHTS 3
layout (location = 0) in vec3 aPos;

struct Transform
{
	mat4 modelToWorld;
	mat4 worldToLightProjection;
};

struct Spot_Light
{
	mat4 projection;
};
uniform Spot_Light spot_light[NUM_LIGHTS];

uniform Transform transform;

void main()
{
	gl_Position = 
		spot_light[0].projection * 
		transform.modelToWorld * 
		vec4(aPos, 1.0);
}