#version 440 core

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

struct Light
{
	vec3 position;
	float brightness;
	vec3 color;
};
struct Camera
{
	vec3 position;
};

uniform Transform transform;
uniform Light light = { vec3(0.0), 10.0, vec3(1.0) };
uniform Camera camera;


// // OUTS // //
out vec3 worldSpace_vertexPosition;
out vec3 worldSpace_vertexNormal;

out vec3 camSpace_vertexPosition;
out vec3 camSpace_camDirection;
out vec3 camSpace_lightDirection;
out vec3 camSpace_normalDirection;

out vec3 tangentSpace_camPosition;
out vec3 tangentSpace_lightPosition;
out vec3 tangentSpace_fragPosition;


void send_worldSpaceCoordinates()
{
	worldSpace_vertexPosition	= (transform.modelToWorld * vec4(model_vertexPosition, 1.0)).xyz;
	worldSpace_vertexNormal		= transform.modelToWorld_normal * model_vertexNormal;

}

void send_camSpaceCoordinates()
{
	camSpace_vertexPosition		= ((transform.worldToCam * vec4(worldSpace_vertexPosition, 1.0)).xyz);
	camSpace_camDirection		= (- camSpace_vertexPosition);
	camSpace_lightDirection		= ((transform.worldToCam * vec4(light.position, 1.0)).xyz - camSpace_vertexPosition);
	camSpace_normalDirection	= ((transform.worldToCam * vec4(worldSpace_vertexNormal, 0.0)).xyz);
	
	//
	// Side not about vectors:
	// The w value should be '1' for points and '0' for vectors
	// Points can be translated and vectors can't
	//
}

void send_tangentSpaceCoordinates()
{
//	vec3 tangent_basis	= normalize(vec3(transform.modelToWorld * vec4(model_vertexTangent, 0.0)));
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

	tangentSpace_camPosition	= worldToTangent * camera.position;
	tangentSpace_lightPosition	= worldToTangent * light.position;
	tangentSpace_fragPosition	= worldToTangent * worldSpace_vertexPosition;
}


void main()
{
	send_worldSpaceCoordinates();
	send_camSpaceCoordinates();
	send_tangentSpaceCoordinates();
	gl_Position = transform.modelToProjection * vec4(model_vertexPosition, 1.0);
}