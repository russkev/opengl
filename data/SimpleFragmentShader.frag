#version 330 core

// // Interpolate values from the vertex shaders // //
in vec3 fragmentColor;

// // Output data // //
out vec4 color;

void main(){

	// // Output colour = colour specified in the vertex shader,
	// // interpolated between all 3 surrounding vertices
	color = vec4(fragmentColor, 1.0);
}