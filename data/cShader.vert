#version 440 core
#define NUM_LIGHTS 5

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
};
uniform Transform transform;

struct Point_Light
{
	vec3 position;
	float brightness;
	vec3 color;
};
uniform Point_Light point_light[NUM_LIGHTS];

struct Directional_Light
{
	vec3 direction;
	float brightness;
	vec3 color;
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
};
uniform Spot_Light spot_light[NUM_LIGHTS];

struct Camera
{
	vec3 position;
};
uniform Camera camera;


// // OUTS // //
out vec3 worldSpace_vertex_position;
out vec3 worldSpace_vertex_normal;

out vec3 camSpace_vertex_position;
out vec3 camSpace_cam_direction;
out vec3 camSpace_light_direction[NUM_LIGHTS];
out vec3 camSpace_normalDirection;

out vec3 tangentSpace_cam_position;
out vec3 tangentSpace_pointLight_position[NUM_LIGHTS];
out vec3 tangentSpace_directionalLight_direction[NUM_LIGHTS];
out vec3 tangentSpace_spotLight_position[NUM_LIGHTS];
out vec3 tangentSpace_frag_position;

out vec2 uv;

void send_uvCoordinates()
{
	uv = model_uv;
}

void send_worldSpaceCoordinates()
{
	worldSpace_vertex_position	= (transform.modelToWorld * vec4(model_vertexPosition, 1.0)).xyz;
	worldSpace_vertex_normal		= transform.modelToWorld_normal * model_vertexNormal;

}

void send_camSpaceCoordinates()
{
	camSpace_vertex_position		= ((transform.worldToCam * vec4(worldSpace_vertex_position, 1.0)).xyz);
	camSpace_cam_direction		= (- camSpace_vertex_position);
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		camSpace_light_direction[i] = ((transform.worldToCam * vec4(point_light[i].position, 1.0)).xyz - camSpace_vertex_position);
	}
	camSpace_normalDirection	= ((transform.worldToCam * vec4(worldSpace_vertex_normal, 0.0)).xyz);
	
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

	tangentSpace_cam_position	= worldToTangent * camera.position;
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		tangentSpace_pointLight_position[i]			= worldToTangent * point_light[i].position;
		tangentSpace_directionalLight_direction[i]	= worldToTangent * directional_light[i].direction;
		tangentSpace_spotLight_position[i]			= worldToTangent * spot_light[i].position;
	}
	tangentSpace_frag_position	= worldToTangent * worldSpace_vertex_position;
}


void main()
{
	send_uvCoordinates();
	send_worldSpaceCoordinates();
	send_camSpaceCoordinates();
	send_tangentSpaceCoordinates();
	gl_Position = transform.modelToProjection * vec4(model_vertexPosition, 1.0);
}