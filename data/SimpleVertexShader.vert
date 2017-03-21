#version 330 core

// // Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
// // Notice that the "1" here matches the "1" in glVertexAttribPointer
layout(location = 1) in vec3 vertexColor;

// // Output data ; will be interpolated for each fragment
out vec3 fragmentColor;

// // Values that stay constant for whole mesh
uniform mat4 MVP;

void main(){

	// // Output position of the vertex, in clip space : MVP * position
	gl_Position = MVP * vec4(vertexPosition_modelspace,1);

	// // The colour of each vertex will be interpolated to produce the colour of each fragment
	fragmentColor = vertexColor;
}