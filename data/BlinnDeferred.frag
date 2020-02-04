#version 440 core
#define NUM_LIGHTS 3

// // ----- INS ----- // //
in vec2 uv;

// // ----- UNIFORMS ----- // //
uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_diffuse_spec;
//uniform vec3 view_position;


struct Light
{
	vec3 position;
	vec3 color;
};
uniform Light lights[NUM_LIGHTS];

struct Camera
{
	vec3 position;
};
uniform Camera camera;

// // ----- HELPER METHODS ----- // //
float dot_product(vec3 a, vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

// // ----- DIFFUSE LIGHTING ----- // //
float diffuse_lighting(vec3 light_direction, vec3 normal)
{
	float cos_theta = dot(light_direction, normal);
	return clamp(cos_theta, 0.0, 1.0);
}

// // ----- SPECULAR LIGHTING ----- // //
float specular_lighting(vec3 light_direction, vec3 normal, vec3 cam_direction)
{
	vec3 reflection = reflect(-light_direction, normal);
	float cos_theta = clamp(dot_product(reflection, cam_direction), 0, 1);
	return pow(cos_theta, 10);
}

// // ----- OUTS ----- // //
out vec4 frag_color;

// // ----- MAIN ----- // //
void main()
{
	vec3 frag_position = texture(g_position, uv).rgb;
	vec3 normal = texture(g_normal, uv).rgb;
	vec3 diffuse = texture(g_diffuse_spec, uv).rgb;
	float specular = texture(g_diffuse_spec, uv).a;

	vec3 cam_direction = normalize(camera.position - frag_position);

	vec3 diffuse_out = vec3(0.0);
	vec3 specular_out = vec3(0.0);

	for (int i = 0; i < NUM_LIGHTS; ++i)
	{
		vec3 light_direction = normalize(lights[i].position - frag_position);
		float diffuse_amount = diffuse_lighting(light_direction, normal);
		float specular_amount = specular_lighting(light_direction, normal, cam_direction);

		diffuse_out += diffuse_amount * diffuse * lights[i].color;
		specular_out += specular_amount * lights[i].color;
	}


	
	vec3 out_color  = 
		diffuse_out + 
		specular_out;
	
	frag_color = vec4(out_color, 1.0);
}
