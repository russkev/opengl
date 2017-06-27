#version 330 core

// // Input vertex data, different for all executions of this shader.
in layout(location = 0) vec3 vertexPosition;
// // Notice that the "1" here matches the "1" in glVertexAttribPointer
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 vertexNormal;
in layout(location = 3) mat4 MVP;

// // Output data ; will be interpolated for each fragment
out vec3 fragmentColor;
out vec4 f_vertexNormal;
out vec4 f_vertexPosition;

// // Values that stay constant for whole mesh
//uniform mat4 MVP;



void main(){
	//vertexPosition_modelspace.x+=offset;
	// // Output position of the vertex, in clip space : MVP * position
	gl_Position			= MVP * vec4(vertexPosition, 1);
	//vec3 lightVector = normalize(lightPosition - vertexPosition);

	// // The colour of each vertex will be interpolated to produce the colour of each fragment
	//float brightness = dot(lightVector, vertexNormal);
	//fragmentColor = vec3(brightness, brightness, brightness);
	f_vertexNormal		= vec4(vertexNormal, 1);
	f_vertexPosition	= vec4(vertexPosition, 1);
}