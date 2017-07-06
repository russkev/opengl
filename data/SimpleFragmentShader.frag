#version 330 core

// // Interpolate values from the vertex shaders // //
//in vec3 fragmentColor;
in vec3 f_world_vertexNormal;
in vec3 f_world_vertexPosition;

// // Uniforms // //
uniform vec4 ambientLight;
uniform vec3 lightPosition;
uniform vec3 camPosition;

// // Output data // //
out vec4 color;

void main(){
	vec3 diff_color				= vec3(1.0, 0.7, 0.5);

	vec3  spec_color			= vec3(1.0, 0.7, 0.5);
	float spec_cosinePower		= 30;

	float lightDistance			= distance(lightPosition, f_world_vertexPosition);
	float lightRadius			= 1;

	float lightIntensity		= 2;
	lightIntensity				= lightIntensity / pow(((lightDistance / lightRadius) + 1), 2);


	vec3 world_lightVector		= normalize(lightPosition - f_world_vertexPosition);
	vec3 world_normalVector		= normalize(f_world_vertexNormal);

	float diff_brightness		= dot(world_lightVector, world_normalVector);
	vec4 diff_light				= vec4(diff_color * diff_brightness, 1.0);

	vec3 reflectedLightVector	= reflect(-world_lightVector, world_normalVector);
	vec3 camVector				= normalize(camPosition - f_world_vertexPosition);

	float spec_brightness		= clamp(dot(reflectedLightVector, camVector), 0, 1);
	spec_brightness				= pow(spec_brightness, spec_cosinePower);
	vec4 spec_light				= vec4(spec_color*spec_brightness, 1.0);

	color						= ambientLight + clamp(diff_light*lightIntensity, 0, 1) + clamp(spec_light, 0, 1);
	//color						= vec4(lightIntensity, lightIntensity, lightIntensity, 1.0);
}