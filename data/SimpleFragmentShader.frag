#version 440 core

// // Interpolate values from the vertex shaders // //
in vec3 f_world_vertexPosition;
in vec3 fragmentColor;
in vec3 f_world_vertexNormal;
in vec2 uv;
in vec3 worldSpace_lightPosition;
//in mat4 transform;

in vec3 cameraSpace_eyeDirection;
in vec3 cameraSpace_lightDirection;

in mat3 TBN_matrix;
in vec3 tangentSpace_lightPosition;
in vec3 tangentSpace_eyePosition;
in vec3 tangentSpace_fragPosition;

// // Uniforms // //
uniform vec4		ambientLight;
//uniform vec3		lightPosition;
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
//	vec3 tex_normal				= normalize(texture( normalMap, vec2(uv.x, -uv.y) ).rgb * 2.0 - 1.0) ;
	//vec3 tex_normal				= texture(normalMap, vec2(uv.x, -uv.y)).rgb;
	vec3 tex_normal				= texture(normalMap, uv).rgb;
	tex_normal					= normalize(tex_normal * 2.0 - 1.0);
	//tex_normal					= normalize(TBN_matrix * tex_normal);
	vec3 diff_color				= mix(tex_a, tex_b, 0.5);

	// Initialise light properties
	float lightDistance			= distance(worldSpace_lightPosition, f_world_vertexPosition);
	float lightRadius			= 1;
	float lightIntensity		= 300;
	lightIntensity				= lightIntensity / pow(((lightDistance / lightRadius) + 1), 2);

	// Initialise specular attributes
	vec3  spec_color			= vec3(1.0, 0.7, 0.5);
	float spec_cosinePower		= 20;

	// Initialise world vectors
	vec3 world_lightVector		= normalize(worldSpace_lightPosition - f_world_vertexPosition);
	vec3 world_normalVector		= normalize(f_world_vertexNormal);
	vec3 light_direction		= normalize(tangentSpace_lightPosition);

	
	// // ----- DIFFUSE ----- // //

	// Calculate the intensity of the illumination based on angle between the notmal and the light
	float cos_theta					= dot(normalize(world_lightVector), world_normalVector);
	
	vec3 ld = normalize(tangentSpace_lightPosition - tangentSpace_fragPosition);
	//float cos_theta		= max(dot(tex_normal, ld), 0.0);
	//float cos_theta				= dot(tangentSpace_lightPosition, f_world_vertexNormal);

	// Calculate the final illumination colour using the diffuse colour and the illumination intensity
	//vec3 diff_light				= diff_color * cos_theta;
	vec3 diff_light = vec3(1.0, 1.0, 1.0) * cos_theta;


	
	// // ----- SPECULAR ----- // //

	// Eye vector (towards camera)
	vec3 eye_vector				= normalize(cameraSpace_eyeDirection);

	// Calculate the vector representing light direction reflected off the texture normal
	vec3 texture_reflection		= reflect(-light_direction, tex_normal);

	// Calculate the cos of the angle between the eye fragment vector and the texture reflection vector
	float cos_alpha				= clamp( dot(eye_vector, texture_reflection), 0, 1);

	// // ----- NORMAL MAP ----- // //

	// Local normal, in tangent space
	//float tangentSpace_textureNormal = normalize(texture(Nor



	// Calculate the cos of the angle between the surface normal and the light
	//float cosTheta				= clamp( dot( tex_normal, light_direction), 0, 1);



	//vec3 eye_direction			= normalize(tangentSpace_eyeDirection);



;




	//vec3 reflectedLightVector	= reflect(-world_lightVector, world_normalVector);
	//vec3 camVector				= normalize(camPosition - f_world_vertexPosition);

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
//	vec4 colorRGB = 
//
//		// Ambient //
//		ambientLight * vec4(diff_color, 1) +
//
//		// Diffuse //
//		clamp(diff_light * lightIntensity, 0, 1) + 
//
//		// Specular //
//		diff_light * lightIntensity * pow(cos_alpha, spec_cosinePower);

	//vec3 colorRGB = normalize(clamp(vec3(cos_theta, cos_theta, cos_theta), 0, 1));
//	vec3 colorRGB = (vec3(cos_theta, cos_theta, cos_theta));
	//vec3 colorRGB = normalize(light_direction);
	vec3 colorRGB = diff_light;
	//vec3 colorRGB = normalize(world_lightVector);

	float alpha					= 1.0;
	color						= vec4(colorRGB[0], colorRGB[1], colorRGB[2], alpha);
}