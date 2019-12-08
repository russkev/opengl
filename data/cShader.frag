#version 440 core

// // INS // //
in vec3 worldSpace_vertexPosition;
in vec3 worldSpace_vertexNormal;

in vec3 camSpace_vertexPosition;
in vec3 camSpace_camDirection;
in vec3 camSpace_lightDirection;
in vec3 camSpace_normalDirection;

in vec3 tangentSpace_camPosition;
in vec3 tangentSpace_lightPosition;
in vec3 tangentSpace_fragPosition;

in vec2 uv;

// // UNIFORMS // //
struct Material
{
	sampler2D diffuse;
	vec3 specular;
	float spec_power;
};
struct Light
{
	vec3 position;
	float brightness;
	vec3 color;
};
struct Camera
{
	vec3 position;
};

//uniform Material material = { vec3(1.0), vec3(1.0), 32 };
uniform Material material;
uniform Light light = { vec3(0.0), 10.0, vec3(1.0) };
uniform Camera camera;

// // OUTS // //
out vec4 fragColor;

// // LOCALS // //
vec3 camSpace_camNormalized		= normalize(camSpace_camDirection);
vec3 camSpace_lightNormalized	= normalize(camSpace_lightDirection);
vec3 camSpace_normalNormalized	= normalize(camSpace_normalDirection);

vec3 tangentSpace_lightDirection	= normalize(tangentSpace_lightPosition - tangentSpace_fragPosition);
vec3 tangentSpace_normal			= vec3(0.0, 0.0, 1.0);
vec3 tangentSpace_camDirection		= normalize(tangentSpace_camPosition - tangentSpace_fragPosition);



// // UTILITY FUNCTIONS // //
bool vec_is_zero(vec3 v)
{
	return !(v.x > 0.0 && v.y > 0.0 && v.z > 0.0);
}

float dot_vec3(vec3 v1, vec3 v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

float attenuation()
{
	float light_distance = distance(worldSpace_vertexPosition, light.position);
	return 1 / (light_distance * light_distance);
}

// // DIFFUSE LIGHTING // //
vec3 diffuse_lighting(vec3 normal, vec3 light)
{
	float cos_theta = clamp(dot(normal, light), 0, 1);
	return vec3(cos_theta);
}

vec3 diffuse_lighting_worldSpace()
{
	return diffuse_lighting(
		worldSpace_vertexNormal, 
		normalize(light.position));
}

vec3 diffuse_lighting_camSpace()
{
	return diffuse_lighting(
		camSpace_normalNormalized, 
		camSpace_lightNormalized);
}

vec3 diffuse_lighting_tangentSpace()
{
	return diffuse_lighting(
		tangentSpace_normal, 
		tangentSpace_lightDirection);
}

// // SPECULAR LIGHTING // //
vec3 specular_lighting(vec3 light, vec3 normal, vec3 cam)
{
	vec3 reflection = reflect(-light, normal);
	float cos_alpha = clamp( dot_vec3( cam, reflection ), 0, 1);
	return vec3(pow(cos_alpha, material.spec_power));
}

vec3 specular_lighting_worldSpace()
{
	vec3 light		= normalize(light.position - worldSpace_vertexPosition);
	vec3 normal		= normalize(worldSpace_vertexNormal);
	vec3 cam		= normalize(camera.position - worldSpace_vertexPosition);
	return specular_lighting(light, normal, cam);
}

vec3 specular_lighting_camSpace()
{
	return specular_lighting(
		camSpace_lightNormalized,
		camSpace_normalNormalized,
		camSpace_camNormalized);
}

vec3 specular_lighting_tangentSpace()
{
	return specular_lighting(
		tangentSpace_lightDirection, 
		tangentSpace_normal, 
		tangentSpace_camDirection);
}

// // MAIN // //
void main ()
{
	float out_brightness	= light.brightness * light.brightness;
	float light_attenuation	= attenuation();

	vec3 diffuse_lighting = diffuse_lighting_camSpace();
	vec3 specular_lighting = specular_lighting_camSpace();

	vec3 outColor = 
		diffuse_lighting * texture(material.diffuse, uv).rgb * out_brightness * light.color
		+ 
		specular_lighting * material.specular * light.color
		* vec3(1.0);

	fragColor = vec4(outColor.xyz, 1.0);
}
