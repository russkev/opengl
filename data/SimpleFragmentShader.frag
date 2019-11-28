#version 440 core

// // Interpolate values from the vertex shaders // //
in vec3 worldSpace_vertexPosition;
in vec3 worldSpace_vertexNormal;
in vec3 fragmentColor;
in vec2 uv;
//in vec3 worldSpace_lightPosition;
//in mat4 transform;

in vec3 cameraSpace_vertexPosition;
in vec3 cameraSpace_normalDirection;
in vec3 cameraSpace_eyeDirection;
in vec3 cameraSpace_lightDirection;

in mat3 mat_tangentToWorld;
in vec3 tangentSpace_lightPosition;
in vec3 tangentSpace_camPosition;
in vec3 tangentSpace_cDirection;
in vec3 tangentSpace_fragPosition;

// // Uniforms // //
uniform vec4		ambientLight;
uniform vec3		lightPosition;
uniform vec3		camPosition;
layout(location = 0) uniform sampler2D	textureA;
layout(location = 1) uniform sampler2D	textureB;
layout(location = 2) uniform sampler2D  normalMap;

// // Output data // //
out vec4 color;

//vec3 lightPosition = vec3(6.0, 6.0, 0.0);

void main()
{
	// Process textures
	vec3 tex_a					= texture2D( textureA, uv ).rgb;
	vec3 tex_b					= texture2D( textureB, uv ).rgb;
	vec3 tex_normal				= normalize(texture(normalMap, uv).rgb * 2.0 - 1.0);
	vec3 diff_color				= mix(tex_a, tex_b, 0.5);

	// Initialise light properties
	float lightDistance			= distance(lightPosition, worldSpace_vertexPosition);
	float lightRadius			= 1;
	float lightIntensity		= 300;
	lightIntensity				= lightIntensity / pow(((lightDistance / lightRadius) + 1), 2);

	// Initialise specular attributes
	float spec_amount			= 100;
	vec3  spec_color			= vec3(1.0, 0.7, 0.5);
	float spec_cosinePower		= 20;

	// Initialise world vectors
	vec3 worldSpace_lightVector			= normalize(lightPosition - worldSpace_vertexPosition);
	vec3 worldSpace_normalVector			= normalize(worldSpace_vertexNormal);
	vec3 worldSpace_texNormalVector		= normalize(mat_tangentToWorld * tex_normal).xyz;

	vec3 cameraSpace_lightVector	= normalize(cameraSpace_lightDirection);
	vec3 cameraSpace_normalVector	= normalize(cameraSpace_normalDirection);

	vec3 tangentSpace_lightVector	= normalize(tangentSpace_lightPosition - tangentSpace_fragPosition);
	vec3 tangentSpace_normalVector	= tex_normal;
	vec3 tangentSpace_cameraVector	= normalize(tangentSpace_camPosition - tangentSpace_fragPosition);




	vec3 light_direction		= normalize(tangentSpace_lightPosition);

	
	// // ----- DIFFUSE ----- // //

	// Calculate the intensity of the illumination based on angle between the normal and the light
	//float cos_theta					= max(dot(worldSpace_lightVector, worldSpace_texNormalVector), 0.0);
	float cos_theta = max(dot(cameraSpace_lightVector, cameraSpace_normalVector), 0.0);
	//float cos_theta		= max(dot(tangentSpace_lightVector, tangentSpace_normalVector), 0.0);
	


	//float cos_theta				= dot(tangentSpace_lightPosition, worldSpace_vertexNormal);

	// Calculate the final illumination colour using the diffuse colour and the illumination intensity
	vec3 diff_light				= diff_color * cos_theta;
	//vec3 diff_light = vec3(1.0, 1.0, 1.0) * cos_theta;


	
	// // ----- SPECULAR ----- // //
//
//	// Eye vector (towards camera)
//	vec3 eye_vector				= normalize(cameraSpace_eyeDirection);
//
//	// Calculate the vector representing light direction reflected off the texture normal
//	vec3 texture_reflection		= reflect(-worldSpace_lightVector, worldSpace_normalVector);
//
//	// Calculate the cos of the angle between the eye fragment vector and the texture reflection vector
//	float cos_alpha				= clamp( dot(eye_vector, texture_reflection), 0, 1);
//
//	float specular				= spec_amount * pow(clamp(cos_alpha, 0.0, 1.0), spec_cosinePower);

//	vec3 cameraVector = normalize(camPosition - worldSpace_vertexPosition);
//	vec3 reflectionVector = (-(lightPosition - worldSpace_vertexPosition), worldSpace_normalVector);
//	float spec = pow(clamp(dot(cameraVector, reflectionVector), 0.0, 1.0), 32);

	vec3 reflectionVector = reflect(-tangentSpace_lightVector, tangentSpace_normalVector);
	float cos_alpha = clamp( dot(tangentSpace_cameraVector,reflectionVector), 0.0, 1.0);
	float spec = pow(cos_alpha, 12);

	// // ----- NORMAL MAP ----- // //

	// Local normal, in tangent space
	//float tangentSpace_textureNormal = normalize(texture(Nor

	// Calculate the cos of the angle between the surface normal and the light
	//float cosTheta				= clamp( dot( tex_normal, light_direction), 0, 1);

	//vec3 eye_direction			= normalize(tangentSpace_eyeDirection);

	//vec3 reflectedLightVector	= reflect(-worldSpace_lightVector, worldSpace_normalVector);
	//vec3 camVector				= normalize(camPosition - worldSpace_vertexPosition);

	//float spec_brightness		= clamp(dot(reflectedLightVector, camVector), 0, 1);
	//spec_brightness				= pow(spec_brightness, spec_cosinePower);
	//vec4 spec_light				= vec4(spec_color*spec_brightness, 1.0);

	//vec4 colorRGB				= 
	//	// Ambient
	//	ambientLight * vec4(diff_color, 1) + 
	//	// Diffuse
	//	clamp(diff_light * lightIntensity, 0, 1) + // + //* cosTheta + 
	//	// Specular
	//	//clamp(spec_light, 0, 1) * pow(cosAlpha, 5);
	//	cosAlpha;
	//	//clamp(spec_light, 0, 1) * pow(cosAlpha,5);
		
		
	//	//ambientLight * vec4(diff_color, 1) + clamp(diff_light * lightIntensity, 0, 1) + clamp(spec_light, 0, 1);

	//vec4 colorRGB				= vec4(diff_color, 1.0);

	//vec4 colorRGB = clamp(diff_light * lightIntensity, 0, 1);
//	vec3 colorRGB = 
//
//		// Ambient //
//		//ambientLight.xyz * diff_color +
//
//		// Diffuse //
//		clamp(diff_light, 0, 1) + 
//
//		// Specular //
//		//diff_light * pow(cos_alpha, spec_cosinePower);
//		spec;

	//vec3 colorRGB = normalize(clamp(vec3(cos_theta, cos_theta, cos_theta), 0, 1));
	//vec3 colorRGB = (vec3(texture_reflection.z, texture_reflection.z, texture_reflection.z));
	//vec3 colorRGB = normalize(light_direction);
	//vec3 colorRGB = diff_light;
	//vec3 colorRGB = normalize(eye_vector);
	vec3 colorRGB = vec3(1.0, 1.0, 1.0) * spec;

	float alpha					= 1.0;
	color						= vec4(colorRGB[0], colorRGB[1], colorRGB[2], alpha);
}