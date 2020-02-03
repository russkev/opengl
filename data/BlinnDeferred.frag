#version 440 core
#define NUM_LIGHTS 3

// // ----- INS ----- // //
in vec2 uv;

// // ----- UNIFORMS ----- // //
uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_diffuse_spec;
uniform vec3 view_position;


struct Light
{
	vec3 position;
	vec3 color;
};
uniform Light lights[NUM_LIGHTS];

// // ----- DIFFUSE LIGHTING ----- // //
float diffuse_lighting(vec3 light_direction, vec3 normal)
{
	float cos_theta = dot(light_direction, normal);
	return clamp(cos_theta, 0, 1);
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

	float diffuse_amount = diffuse_lighting(normalize(lights[0].position - frag_position), normal);
	frag_color = vec4(vec3(diffuse_amount * diffuse), 1.0);
}
