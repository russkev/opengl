#version 440 core

// // Interpolate values from the vertex shaders // //
in vec3 f_world_vertexPosition;
in vec3 fragmentColor;
in vec3 f_world_vertexNormal;
in vec2 uv;
in vec3 worldSpace_lightPosition;

in vec3 cameraSpace_eyeDirection;
in vec3 cameraSpace_lightDirection;

in vec3 tangentSpace_eyeDirection;
in vec3 tangentSpace_lightDirection;

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
	vec3 tex_a					= texture2D( textureA, uv ).rgb;
	vec3 tex_b					= texture2D( textureB, uv ).rgb;
	vec3 tex_normal				= normalize(texture( normalMap, vec2(uv.x, -uv.y) ).rgb * 2.0 - 1.0) ;

	//vec3 diff_color			= tex_b;
	vec3 diff_color				= mix(tex_a, tex_b, 0.5);
	//vec3 diff_color			= texture2D(textureA, uv).rgb * fragmentColor;

	vec3  spec_color			= vec3(1.0, 0.7, 0.5);
	float spec_cosinePower		= 30;

	float lightDistance			= distance(worldSpace_lightPosition, f_world_vertexPosition);
	float lightRadius			= 1;

	float lightIntensity		= 200;
	lightIntensity				= lightIntensity / pow(((lightDistance / lightRadius) + 1), 2);


	vec3 world_lightVector		= normalize(worldSpace_lightPosition - f_world_vertexPosition);
	vec3 world_normalVector		= normalize(f_world_vertexNormal);

	vec3 light_direction		= normalize(tangentSpace_lightDirection);
	float cosTheta				= clamp( dot( tex_normal, light_direction), 0, 1);

	vec3 eye_direction			= normalize(tangentSpace_eyeDirection);
	vec3 reflection				= reflect(-light_direction, tex_normal);
	float cosAlpha				= clamp( dot(eye_direction, reflection), 0, 1);

	float diff_brightness		= dot(world_lightVector, world_normalVector);
	vec4 diff_light				= vec4(diff_color * diff_brightness, 1.0);

	vec3 reflectedLightVector	= reflect(-world_lightVector, world_normalVector);
	vec3 camVector				= normalize(camPosition - f_world_vertexPosition);

	float spec_brightness		= clamp(dot(reflectedLightVector, camVector), 0, 1);
	spec_brightness				= pow(spec_brightness, spec_cosinePower);
	vec4 spec_light				= vec4(spec_color*spec_brightness, 1.0);

	vec4 colorRGB				= 
		// Ambient
		ambientLight * vec4(diff_color, 1) + 
		// Diffuse
		clamp(diff_light * lightIntensity, 0, 1) + // + //* cosTheta + 
		// Specular
		//clamp(spec_light, 0, 1) * pow(cosAlpha, 5);
		cosAlpha;
		//clamp(spec_light, 0, 1) * pow(cosAlpha,5);
		
		
		//ambientLight * vec4(diff_color, 1) + clamp(diff_light * lightIntensity, 0, 1) + clamp(spec_light, 0, 1);

	//vec4 colorRGB				= vec4(diff_color, 1.0);
	float alpha					= 1.0;
	color						= vec4(colorRGB[0], colorRGB[1], colorRGB[2], alpha);
}