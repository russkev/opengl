#version 330 core

// // Interpolate values from the vertex shaders // //
in vec3 fragmentColor;
in vec3 f_vertexNormal;
in vec4 f_vertexPosition;
in vec3 f_lightPosition;

// // Output data // //
out vec4 color;

void main(){
	vec4 lightVector = normalize(f_lightPosition - f_vertexPosition);
	float brightness = dot(lightVector, f_vertexNormal);


	// // Output colour = colour specified in the vertex shader,
	// // interpolated between all 3 surrounding vertices
	color = vec4(brightness, brightness, brightness, 1.0);
}