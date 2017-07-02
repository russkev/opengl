#version 330 core

// // Interpolate values from the vertex shaders // //
//in vec3 fragmentColor;
in vec3 f_vertexNormal;
//in vec4 f_vertexPosition;
in vec3 f_worldPosition;

// // Uniforms // //
//uniform vec3 ambient;
uniform vec3 lightPosition;
//uniform mat4 worldPosition;
uniform vec3 f_fragmentPosition;


// // Output data // //
out vec4 color;


void main(){
	vec3 lightVector = normalize(lightPosition - f_worldPosition);
	float brightness = dot(lightVector, f_vertexNormal);


	// // Output colour = colour specified in the vertex shader,
	// // interpolated between all 3 surrounding vertices
	color = vec4(brightness, brightness, brightness, 1.0);
	//color = vec4(lightPosition, 1);
	//color = vec4(f_worldPosition, 1);
}