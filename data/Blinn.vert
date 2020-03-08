#version 440 core
#pragma optionNV unroll all
#define NUM_LIGHTS 3

// // ----- INS ----- // //
in layout(location = 0 ) vec3 vertex_position;
in layout(location = 1 ) vec3 vertex_color;
in layout(location = 2 ) vec3 vertex_normal;
in layout(location = 3 ) vec2 vertex_uv;
in layout(location = 4 ) int  vertex_id;
in layout(location = 5 ) vec3 vertex_tangent;
in layout(location = 6 ) vec3 vertex_bitangent;

// // ----- UNIFORMS ----- // //
struct Transform
{
	mat4 model_to_projection;
	mat4 model_to_world;
	mat3 model_to_world_normal;
};
uniform Transform transform;

struct Shadow
{
	bool enabled;
	float bias;
	float radius;
	int num_samples;
};

struct PointLight
{
	vec3 position;
	float brightness;
	vec3 color;
	samplerCube depth;
	float far_plane;
	Shadow shadow;
};
uniform PointLight pointLight[NUM_LIGHTS];

struct DirectionalLight
{
	vec3 direction;
	float brightness;
	vec3 color;
	sampler2DArray depth;
	mat4 projection;
	Shadow shadow;
};
uniform DirectionalLight directionalLight[NUM_LIGHTS];

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float brightness;
	vec3 color;
	float inner;
	float outer;
	sampler2DArray depth;
	mat4 projection;
	Shadow shadow;
};
uniform SpotLight spotLight[NUM_LIGHTS];

struct Camera
{
	vec3 position;
};
uniform Camera camera;

// // ----- OUTS ----- // //
out vec3 test;

out Out_Frag
{
	vec3 world_space_position;
	vec3 world_space_normal;
	vec3 tangent_space_position;
} out_frag;

out Out_Cam
{
	vec3 tangent_space_position;
} out_cam;

out Out_PointLight
{
	vec3 tangent_space_position;
} out_pointLight[NUM_LIGHTS];

out Out_SpotLight
{
	vec4 light_space_position;
	vec3 tangent_space_position;
} out_spotLight[NUM_LIGHTS];

out Out_DirectionalLight
{
	vec4 light_space_position;
	vec3 tangent_space_position;
} out_directionalLight[NUM_LIGHTS];

out vec2 flat_uv;

// // ----- SEND FUNCTIONS ----- // //
void send_uv_coordinates()
{
	flat_uv = vertex_uv;
}

void send_world_space_coordinates()
{
	out_frag.world_space_position	= (transform.model_to_world * vec4(vertex_position, 1.0)).xyz;
	out_frag.world_space_normal		= transform.model_to_world_normal * vertex_normal;
}

void send_tangent_space_coordinates()
{
	vec3 tangent_basis		= normalize((transform.model_to_world * vec4(vertex_tangent, 0.0)).xyz);
	vec3 normal_basis		= normalize((transform.model_to_world * vec4(vertex_normal, 0.0)).xyz);

	//
	// Here Gram Schmidt procedure and cross product are used 
	// to accurately calculate the basis vectors
	//

	tangent_basis			= normalize(tangent_basis - dot(tangent_basis, normal_basis) * normal_basis);
	vec3 bitangent_basis	= cross(normal_basis, tangent_basis);

	mat3 tangent_to_world		= mat3(tangent_basis, bitangent_basis, normal_basis);
	mat3 world_to_tangent		= transpose(tangent_to_world);

	out_cam.tangent_space_position	= world_to_tangent * camera.position;
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		out_pointLight[i].tangent_space_position		= world_to_tangent * pointLight[i].position;
		out_directionalLight[i].tangent_space_position	= world_to_tangent * directionalLight[i].direction;
		out_spotLight[i].tangent_space_position			= world_to_tangent * spotLight[i].position;
	}
	out_frag.tangent_space_position	= world_to_tangent * out_frag.world_space_position;
}

void send_light_space_coordinates()
{
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		out_spotLight[i].light_space_position = spotLight[i].projection * vec4(out_frag.world_space_position, 1.0f);
		out_directionalLight[i].light_space_position = directionalLight[i].projection * vec4(out_frag.world_space_position, 1.0f);
	}
}

// // ----- MAIN ----- // //
void main()
{
	send_uv_coordinates();
	send_world_space_coordinates();
	send_tangent_space_coordinates();
	send_light_space_coordinates();

	gl_Position = transform.model_to_projection * vec4(vertex_position, 1.0);
}