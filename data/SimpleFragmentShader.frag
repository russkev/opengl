#version 330 core

// // Interpolate values from the vertex shaders // //
//in vec3 fragmentColor;
in vec3 f_world_vertexNormal;
in vec3 f_world_vertexPosition;

// // Uniforms // //
uniform vec4 ambientLight;
uniform vec3 lightPosition;
uniform vec3 f_fragmentPosition;
uniform vec3 camPosition;


// // Output data // //
out vec4 color;


void main(){
	vec3 lightVector = normalize(lightPosition - f_world_vertexPosition);
	float brightness = dot(lightVector, normalize(f_world_vertexNormal));
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);
	//color = ambientLight + clamp(diffuseLight, 0, 1);
	color = vec4(0, camPosition[1] - 3.5, 0, 1);
}