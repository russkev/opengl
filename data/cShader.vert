#version 440 core

// // Input vertex data, different for all executions of this shader.
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
uniform vec3 light_position;

// // OUTS // //
out vec3 worldSpace_vertexPosition;
out vec3 worldSpace_vertexNormal;


void send_worldSpaceCoordinates()
{
//	worldSpace_vertexNormal		= model_vertexPosition;
//	worldSpace_vertexNormal		= model_vertexNormal;
	worldSpace_vertexPosition	= vec3(mat_modelToWorld * vec4(model_vertexPosition, 1.0));

//	worldSpace_vertexPosition	= vec3(mat_modelToWorld * vec4(model_vertexPosition, 1.0));

	worldSpace_vertexNormal		= vec3(mat_modelToWorld * vec4(model_vertexNormal, 1.0));
}

void main()
{
	send_worldSpaceCoordinates();
	gl_Position = mat_modelToProjection * vec4(model_vertexPosition, 1.0);
}