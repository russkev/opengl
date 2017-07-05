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
	vec3 world_lightVector		= normalize(lightPosition - f_world_vertexPosition);
	vec3 world_normalVector		= normalize(f_world_vertexNormal);

	float diffuseBrightness		= dot(world_lightVector, world_normalVector);
	vec4 diffuseLight			= vec4(diffuseBrightness, diffuseBrightness, diffuseBrightness, 1.0);

	//vec3 reflectedLightVector	= lightVector - 2 * diffuseBrightness * normalVector;
	vec3 reflectedLightVector	= reflect(-world_lightVector, world_normalVector);
	vec3 camVector				= normalize(camPosition - f_world_vertexPosition);

	float specularBrightness	= clamp(dot(reflectedLightVector, camVector), 0, 1);
	vec4 specularLight			= vec4(specularBrightness, specularBrightness, specularBrightness, 1.0);

	//color = ambientLight + clamp(diffuseLight, 0, 1);
	color = specularLight;
}