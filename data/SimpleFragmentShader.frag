#version 330 core

// // Interpolate values from the vertex shaders // //
in vec3 fragmentColor;
in vec4 f_vertexNormal;
in vec4 f_vertexPosition;

// // Uniforms // //
uniform vec3 ambient;
uniform vec3 lightPosition;
uniform vec3 worldPosition;


// // Output data // //
out vec4 color;


void main(){
	vec4 lightVector = normalize(vec4(lightPosition, 1) - f_vertexPosition);
	float brightness = dot(lightVector, f_vertexNormal);


	// // Output colour = colour specified in the vertex shader,
	// // interpolated between all 3 surrounding vertices
	color = vec4(brightness, brightness, brightness, 1.0);
}