#version 330 core

// // Interpolate values from the vertex shaders // //
//in vec3 fragmentColor;
in vec3 f_vertexNormal;
in vec3 f_worldPosition;

// // Uniforms // //
//uniform vec3 ambient;
uniform vec3 lightPosition;
uniform vec3 f_fragmentPosition;


// // Output data // //
out vec4 color;


void main(){
	vec3 lightVector = normalize(lightPosition - f_worldPosition);
	float brightness = dot(lightVector, f_vertexNormal);
	color = vec4(brightness, brightness, brightness, 1.0);
}