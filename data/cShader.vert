#version 440 core
#define NUM_LIGHTS 3


// // INS // //
in layout(location = 0 ) vec3 model_vertexPosition;
in layout(location = 1 ) vec3 model_vertexColor;
in layout(location = 2 ) vec3 model_vertexNormal;
in layout(location = 3 ) vec2 model_uv;
in layout(location = 4 ) int  model_id;
in layout(location = 5 ) vec3 model_vertexTangent;
in layout(location = 6 ) vec3 model_vertexBitangent;


// // UNIFORMS // //
struct Transform
{
	mat4 modelToProjection;
	mat4 modelToWorld;
	mat3 modelToWorld_normal;
	mat4 worldToCam;
	//mat4 worldToLightProjection;
};
uniform Transform transform;

struct Point_Light
{
	vec3 position;
	float brightness;
	vec3 color;
	samplerCube depth;
	mat4 projection;
};
uniform Point_Light point_light[NUM_LIGHTS];

struct Directional_Light
{
	vec3 direction;
	float brightness;
	vec3 color;
	sampler2DArray depth;
	mat4 projection;
};
uniform Directional_Light directional_light[NUM_LIGHTS];

struct Spot_Light
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
uniform Spot_Light spot_light[NUM_LIGHTS];

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

void send_uvCoordinates()
{
	uv = model_uv;
}

void send_worldSpaceCoordinates()
{
	worldSpace.vertex_position	= (transform.modelToWorld * vec4(model_vertexPosition, 1.0)).xyz;
	worldSpace.vertex_normal		= transform.modelToWorld_normal * model_vertexNormal;

}

void send_camSpaceCoordinates()
{
	camSpace.vertex_position		= ((transform.worldToCam * vec4(worldSpace.vertex_position, 1.0)).xyz);
	camSpace.cam_direction		= (- camSpace.vertex_position);
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		camSpace.light_direction[i] = ((transform.worldToCam * vec4(point_light[i].position, 1.0)).xyz - camSpace.vertex_position);
	}
	camSpace.normal_direction	= ((transform.worldToCam * vec4(worldSpace.vertex_normal, 0.0)).xyz);
	
	//
	// Side not about vectors:
	// The w value should be '1' for points and '0' for vectors
	// Points can be translated and vectors can't
	//
}

void send_tangentSpaceCoordinates()
{
	vec3 tangent_basis		= normalize((transform.modelToWorld * vec4(model_vertexTangent, 0.0)).xyz);
	vec3 normal_basis		= normalize((transform.modelToWorld * vec4(model_vertexNormal, 1.0)).xyz);

	//
	// Here Gram Schmidt procedure and cross product are used 
	// to accurately calculate the basis vectors
	//

	tangent_basis			= normalize(tangent_basis - dot(tangent_basis, normal_basis) * normal_basis);
	vec3 bitangent_basis	= cross(normal_basis, tangent_basis);


	mat3 tangentToWorld		= mat3(tangent_basis, bitangent_basis, normal_basis);
	mat3 worldToTangent		= transpose(tangentToWorld);

	tangentSpace.cam_position	= worldToTangent * camera.position;
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		tangentSpace.pointLight_position[i]			= worldToTangent * point_light[i].position;
		tangentSpace.directionalLight_direction[i]	= worldToTangent * directional_light[i].direction;
		tangentSpace.spotLight_position[i]			= worldToTangent * spot_light[i].position;
	}
	tangentSpace.frag_position	= worldToTangent * worldSpace.vertex_position;
}

void send_lightSpaceCoordinates()
{
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		spotLight_space.position[i] = spot_light[i].projection * vec4(worldSpace.vertex_position, 1.0f);
		directionalLight_space.position[i] = directional_light[i].projection * vec4(worldSpace.vertex_position, 1.0f);
		pointLight_space.position[i] = point_light[i].projection * vec4(worldSpace.vertex_position, 1.0f);
	}
}

void main()
{
	send_uvCoordinates();
	send_worldSpaceCoordinates();
	send_camSpaceCoordinates();
	send_tangentSpaceCoordinates();
	//lightSpace.position = transform.worldToLightProjection * vec4(worldSpace.vertex_position, 1.0f);
	send_lightSpaceCoordinates();
	gl_Position = transform.modelToProjection * vec4(model_vertexPosition, 1.0);
}