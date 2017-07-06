#version 330 core

// // Input vertex data, different for all executions of this shader.
in layout(location = 0) vec4 model_vertexPosition;
in layout(location = 1) vec3 model_vertexColor;
in layout(location = 2) vec3 model_vertexNormal;
//in layout(location = 3) vec4 model_camPosition;
in layout(location = 4) mat4 mat_modelToProjection;
in layout(location = 8) mat4 mat_modelToWorld;

// // Output data ; will be interpolated for each fragment
out vec3 f_world_vertexNormal;
out vec3 f_world_vertexPosition;

// // Uniforms ; values that stay constant for whole mesh

void main(){
	// // Output position of the vertex, in clip space : MVP * position
	gl_Position				= mat_modelToProjection * model_vertexPosition;

	// // The colour of each vertex will be interpolated to produce the colour of each fragment
	f_world_vertexNormal	= normalize(vec3(mat_modelToWorld * vec4(model_vertexNormal, 0)));
	f_world_vertexPosition	= vec3(mat_modelToWorld * model_vertexPosition);
}