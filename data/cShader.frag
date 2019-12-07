#version 440 core

// // INS // //
in vec3 worldSpace_vertexPosition;
in vec3 worldSpace_vertexNormal;

in vec3 camSpace_vertexPosition;
in vec3 camSpace_camDirection;
in vec3 camSpace_lightDirection;
in vec3 camSpace_normalDirection;

// // UNIFORMS // //
uniform vec4 uColor;
uniform vec3 light_position;
uniform vec3 cam_position;
uniform float light_brightness = 4;

// // OUTS // //
out vec4 fragColor;

// // UTILITY FUNCTIONS // //
bool vec_is_zero(vec3 v)
{
	return !(v.x > 0.0 && v.y > 0.0 && v.z > 0.0);
}

float attenuation()
{
	float light_distance = distance(worldSpace_vertexPosition, light_position);
	return 1 / (light_distance * light_distance);
}

// // DIFFUSE LIGHTING // //
vec3 diffuse_lighting_worldSpace()
{
	float cos_theta = clamp(dot(worldSpace_vertexNormal, normalize(light_position)), 0, 1);
	return vec3(cos_theta);
}

vec3 diffuse_lighting_camSpace()
{
	float cos_theta = clamp(dot(camSpace_normalDirection, normalize(camSpace_lightDirection)), 0.0, 1.0);
	return vec3(cos_theta);
}

// // SPECULAR LIGHTING // //
vec3 specular_lighting_worldSpace(vec3 diffuse_lighting)
{
	if (vec_is_zero(diffuse_lighting))
	{
		return vec3(0.0);
	}
	else
	{
		vec3 light		= normalize(light_position - worldSpace_vertexPosition);
		vec3 normal		= normalize(worldSpace_vertexNormal);
		vec3 cam		= normalize(cam_position - worldSpace_vertexPosition);
		vec3 reflection = reflect(-light, normal);
		float cos_alpha = clamp( dot( cam, reflection ), 0, 1);
		return vec3(pow(cos_alpha, 20));
	}
}

vec3 specular_lighting_camSpace(vec3 diffuse_lighting)
{
	if (vec_is_zero(diffuse_lighting))
	{
		return vec3(0.0);
	}
	else
	{
		vec3 light		= normalize(camSpace_lightDirection);
		vec3 normal		= normalize(camSpace_normalDirection);
		vec3 cam		= normalize(camSpace_camDirection);
		vec3 reflection = reflect(-light, normal);
		float cos_alpha = clamp( dot( cam, reflection ), 0, 1);
		return vec3(pow(cos_alpha, 20));
	}
}

// // MAIN // //
void main ()
{
	float light_attenuation = attenuation();



	vec3 diffuse_lighting = diffuse_lighting_camSpace();
	vec3 specular_lighting = specular_lighting_camSpace(diffuse_lighting);

	vec3 outColor = 
		worldSpace_vertexNormal
		//light_brightness * diffuse_lighting * light_attenuation
		//+ 
		//specular_lighting
		* vec3(1.0);
	fragColor = vec4(outColor.xyz, 1.0);
}
