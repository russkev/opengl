#version 330 core

// // Input vertex data, different for all executions of this shader.
in layout(location = 0) vec3 vertexPosition;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 vertexNormal;
in layout(location = 3) mat4 ModelMatrix;
in layout(location = 7) mat4 WorldMatrix;

// // Output data ; will be interpolated for each fragment
out vec3 f_vertexNormal;
out vec3 f_worldPosition;

// // Uniforms ; values that stay constant for whole mesh

void main(){
	// // Output position of the vertex, in clip space : MVP * position
	gl_Position			= ModelMatrix * vec4(vertexPosition, 1);
	vec4 v				= vec4(vertexPosition, 1);

	// // The colour of each vertex will be interpolated to produce the colour of each fragment
	f_vertexNormal		= normalize(vec3(WorldMatrix * vec4(vertexNormal, 0)));
	f_worldPosition		= vec3(WorldMatrix * v);
}