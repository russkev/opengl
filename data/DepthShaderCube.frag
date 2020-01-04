#version 440 core
#define NUM_LIGHTS 3
#define NUM_FACES 6

in vec4 frag_position;

//uniform mat4 shadow_transforms[NUM_FACES];

struct Point_Light
{
	vec3 position;
	float far_plane;
};
uniform Point_Light point_light;

void main()
{
//	mat4 temp = mat4(1.0);
//
//	for (int i = 0; i < NUM_FACES; i++)
//	{
//		temp = (shadow_transforms[i] * temp);
//	}
//

	// Get distance between fragment and light source
	float light_distance = length(frag_position.xyz - point_light.position);
//	float light_distance = length((frag_position * temp).xyz - point_light.position);

	// Map to [0;1] range by dividing by range
	light_distance = light_distance / point_light.far_plane;

	// Write this as modified depth
	gl_FragDepth = light_distance;
}
