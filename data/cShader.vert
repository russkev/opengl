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
uniform mat4 mat_modelToProjection;
uniform mat4 mat_modelToWorld;
uniform mat3 mat_modelToWorld_normal;
uniform mat4 mat_worldToCam;
uniform vec3 light_position;
uniform vec3 cam_position;


// // OUTS // //
out vec3 worldSpace_vertexPosition;
out vec3 worldSpace_vertexNormal;

out vec3 camSpace_vertexPosition;
out vec3 camSpace_camDirection;
out vec3 camSpace_lightDirection;
out vec3 camSpace_normalDirection;


void send_worldSpaceCoordinates()
{
	worldSpace_vertexPosition	= (mat_modelToWorld * vec4(model_vertexPosition, 1.0)).xyz;
	worldSpace_vertexNormal		= (mat_modelToWorld * vec4(model_vertexNormal,   0.0)).xyz;
	//worldSpace_vertexNormal		= mat_modelToWorld_normal * model_vertexNormal;

}

void send_camSpaceCoordinates()
{
	camSpace_vertexPosition		= ((mat_worldToCam * vec4(worldSpace_vertexPosition, 1.0)).xyz);
	camSpace_camDirection		= (- camSpace_vertexPosition);
	camSpace_lightDirection		= ((mat_worldToCam * vec4(light_position, 1.0)).xyz - camSpace_vertexPosition);
	camSpace_normalDirection	= ((mat_worldToCam * vec4(worldSpace_vertexNormal, 0.0)).xyz);
	
	//
	// Side not about vectors:
	// The w value should be '1' for points and '0' for vectors
	// Points can be translated and vectors can't
	//
}

void main()
{
	send_worldSpaceCoordinates();
	send_camSpaceCoordinates();
	gl_Position = mat_modelToProjection * vec4(model_vertexPosition, 1.0);
}