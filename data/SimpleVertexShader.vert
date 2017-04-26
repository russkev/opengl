#version 330 core

// // Input vertex data, different for all executions of this shader.
in layout(location = 0) vec3 vertexPosition;
// // Notice that the "1" here matches the "1" in glVertexAttribPointer
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) float offset;

// // Output data ; will be interpolated for each fragment
out vec3 fragmentColor;

// // Values that stay constant for whole mesh
uniform mat4 MVP;
uniform float darken;


void main(){
	//vertexPosition_modelspace.x+=offset;
	// // Output position of the vertex, in clip space : MVP * position
	gl_Position = MVP * vec4(vertexPosition.x + (offset*2), vertexPosition.y, vertexPosition.z,1);

	// // The colour of each vertex will be interpolated to produce the colour of each fragment
	fragmentColor = vertexColor*darken;
}