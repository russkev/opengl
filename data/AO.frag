#version 440 core
#define NUM_SAMPLES 128

// // ----- INS ----- // //
in vec2 uv;

// // ----- UNIFORMS ----- // //
uniform sampler2D g_cam_space_position;
uniform sampler2D g_cam_space_normal;
uniform sampler2D noise;

uniform vec3 samples[NUM_SAMPLES];
//struct Samples
//{
//	vec3 samp;
//};
//uniform Samples samples[4];

uniform float radius;
uniform float bias;

uniform uvec2 screen_dimensions;
uniform uvec2 noise_tile_dimensions;

uniform mat4 cam_to_projection;

// // ----- OUTS ----- // //
out vec4 frag_color;

// // ----- LOCALS ----- // //
const vec2 m_noise_scale = vec2(
	screen_dimensions.x / noise_tile_dimensions.x,
	screen_dimensions.y / noise_tile_dimensions.y);

struct m_Cam_Space
{
	vec3 position;
	vec3 normal;
} m_cam_space;

vec3 m_randomized_vector;

// // ----- INITIALIZATION OF LOCALS ----- // //
void init()
{
	m_cam_space.position = texture(g_cam_space_position, uv).xyz;
	m_cam_space.normal = texture(g_cam_space_normal, uv).xyz;
	m_randomized_vector = texture(noise, uv * m_noise_scale).xyz;
}

mat3 tangent_space_basis(vec3 normal, vec3 randomized_vector)
{
	vec3 tangent = normalize(randomized_vector - normal * dot(randomized_vector, normal));
	vec3 bitangent = cross(normal, tangent);
	return mat3(tangent, bitangent, normal);
}


// // ----- MAIN ----- // //
void main()
{
	vec3 out_color;
	init();

	mat3 tbn = tangent_space_basis(m_cam_space.normal, m_randomized_vector);

	float occlusion = 0.0;

//	vec4 offset = vec4(0.0);
	for (int i = 0; i < NUM_SAMPLES; ++i)
	{
		vec3 ao_sample = tbn * samples[i];
		ao_sample = m_cam_space.position + ao_sample * radius;

		vec4 offset = vec4(ao_sample, 1.0);
		offset = cam_to_projection * offset;
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;

		float sample_depth = texture(g_cam_space_position, offset.xy).z;

		float range_check = smoothstep(0.0, 1.0, radius / abs(m_cam_space.position.z - sample_depth));
		occlusion += (sample_depth >= ao_sample.z + bias ? 1.0 : 0.0) * range_check;
	}

	occlusion = 1.0 - (occlusion / NUM_SAMPLES);

	out_color = vec3(occlusion);

	frag_color = vec4(out_color, 1.0);
}

