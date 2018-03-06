#version 440 core

// // Interpolate values from the vertex shaders // //
in vec3 f_world_vertexPosition;
in vec3 fragmentColor;
in vec3 f_world_vertexNormal;


// // Uniforms // //
uniform vec4 ambientLight;
uniform vec3 lightPosition;
uniform vec3 camPosition;

// // Subroutine tutorial
uniform int isRed;
// Subroutine signature
subroutine vec4 colorRedBlue ();

// Option 1
subroutine (colorRedBlue) vec4 redColor() {
	return vec4(1.0, 0.0, 0.0, 1.0); }
// Option 2
subroutine (colorRedBlue) vec4 blueColor() {
	return vec4(0.0, 0.0, 1.0, 1.0); }

subroutine uniform colorRedBlue myRedBlueSelection;

// // End subroutine


layout (binding = 1, offset = 0) uniform atomic_uint atRed;
layout (binding = 1, offset = 4) uniform atomic_uint atGreen;
layout (binding = 1, offset = 8) uniform atomic_uint atBlue;

// // Output data // //
out vec4 color;


void main()
{
	color = myRedBlueSelection();



	/*
	if ((fragmentColor[0] >= fragmentColor[1]) && (fragmentColor[0] >= fragmentColor[2]))
	{
		atomicCounterIncrement(atRed);
	}
	else if (fragmentColor[1] >= fragmentColor[2])
	{
		atomicCounterIncrement(atGreen);
	}
	else
	{
		atomicCounterIncrement(atBlue);
	}
	
	vec3 diff_color				= fragmentColor;

	vec3  spec_color			= vec3(1.0, 0.7, 0.5);
	float spec_cosinePower		= 30;

	float lightDistance			= distance(lightPosition, f_world_vertexPosition);
	float lightRadius			= 1;

	float lightIntensity		= 200;
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

	vec4 colorRGB				= ambientLight * vec4(diff_color, 1) + clamp(diff_light * lightIntensity, 0, 1) + clamp(spec_light, 0, 1);
	float alpha					= 0.5;
	color						= vec4(colorRGB[0], colorRGB[1], colorRGB[2], alpha);
	color						= ambientLight;
	*/

}