#version 440 core
#define NUM_LIGHTS 3


// // INS // //
in layout(location = 0 ) vec3 vertex_position;
in layout(location = 1 ) vec3 vertex_color;
in layout(location = 2 ) vec3 vertex_normal;
in layout(location = 3 ) vec2 vertex_uv;
in layout(location = 4 ) int  vertex_id;
in layout(location = 5 ) vec3 vertex_tangent;
in layout(location = 6 ) vec3 vertex_bitangent;


// // UNIFORMS // //
struct Transform
{
	mat4 model_to_projection;
	mat4 model_to_world;
	mat3 model_to_world_normal;
	mat4 world_to_cam;
};
uniform Transform transform;

struct PointLight
{
	vec3 position;
	float brightness;
	vec3 color;
	samplerCube depth;
	mat4 projection;
	float far_plane;
};
uniform PointLight pointLight[NUM_LIGHTS];

struct DirectionalLight
{
	vec3 direction;
	float brightness;
	vec3 color;
	sampler2DArray depth;
	mat4 projection;
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
};
uniform SpotLight spotLight[NUM_LIGHTS];

struct Camera
{
	vec3 position;
};
uniform Camera camera;


// // OUTS // //
out WorldSpace
{
	vec3 vertex_position;
	vec3 vertex_normal;
} worldSpace;

out CamSpace
{
	vec3 vertex_position;
	vec3 cam_direction;
	vec3 light_direction[NUM_LIGHTS];
	vec3 normal_direction;
} camSpace;

out TangentSpace
{
	vec3 cam_position;
	vec3 pointLight_position[NUM_LIGHTS];
	vec3 directionalLight_direction[NUM_LIGHTS];
	vec3 spotLight_position[NUM_LIGHTS];
	vec3 frag_position;
} tangentSpace;

out PointLight_Space
{
	vec4 position[NUM_LIGHTS];
} pointLight_space;

out SpotLight_Space
{
	vec4 position[NUM_LIGHTS];
} spotLight_space;

out DirectionalLight_Space
{
	vec4 position[NUM_LIGHTS];
} directionalLight_space;

out vec2 uv;

void send_uv_coordinates()
{
	uv = vertex_uv;
}

void send_world_space_coordinates()
{
	worldSpace.vertex_position	= (transform.model_to_world * vec4(vertex_position, 1.0)).xyz;
	worldSpace.vertex_normal		= transform.model_to_world_normal * vertex_normal;

}

void send_cam_space_coordinates()
{
	camSpace.vertex_position		= ((transform.world_to_cam * vec4(worldSpace.vertex_position, 1.0)).xyz);
	camSpace.cam_direction		= (- camSpace.vertex_position);
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		camSpace.light_direction[i] = ((transform.world_to_cam * vec4(pointLight[i].position, 1.0)).xyz - camSpace.vertex_position);
	}
	camSpace.normal_direction	= ((transform.world_to_cam * vec4(worldSpace.vertex_normal, 0.0)).xyz);
	
	//
	// Side not about vectors:
	// The w value should be '1' for points and '0' for vectors
	// Points can be translated and vectors can't
	//
}

void send_tangent_space_coordinates()
{
	vec3 tangent_basis		= normalize((transform.model_to_world * vec4(vertex_tangent, 0.0)).xyz);
	vec3 normal_basis		= normalize((transform.model_to_world * vec4(vertex_normal, 1.0)).xyz);

	//
	// Here Gram Schmidt procedure and cross product are used 
	// to accurately calculate the basis vectors
	//

	tangent_basis			= normalize(tangent_basis - dot(tangent_basis, normal_basis) * normal_basis);
	vec3 bitangent_basis	= cross(normal_basis, tangent_basis);


	mat3 tangent_to_world		= mat3(tangent_basis, bitangent_basis, normal_basis);
	mat3 world_to_tangent		= transpose(tangent_to_world);

	tangentSpace.cam_position	= world_to_tangent * camera.position;
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		tangentSpace.pointLight_position[i]			= world_to_tangent * pointLight[i].position;
		tangentSpace.directionalLight_direction[i]	= world_to_tangent * directionalLight[i].direction;
		tangentSpace.spotLight_position[i]			= world_to_tangent * spotLight[i].position;
	}
	tangentSpace.frag_position	= world_to_tangent * worldSpace.vertex_position;
}

void send_light_space_coordinates()
{
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		spotLight_space.position[i] = spotLight[i].projection * vec4(worldSpace.vertex_position, 1.0f);
		directionalLight_space.position[i] = directionalLight[i].projection * vec4(worldSpace.vertex_position, 1.0f);
		pointLight_space.position[i] = pointLight[i].projection * vec4(worldSpace.vertex_position, 1.0f);
	}
}

void main()
{
	send_uv_coordinates();
	send_world_space_coordinates();
	send_cam_space_coordinates();
	send_tangent_space_coordinates();
	send_light_space_coordinates();
	gl_Position = transform.model_to_projection * vec4(vertex_position, 1.0);
}