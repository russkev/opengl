#version 440 core
#define NUM_LIGHTS 10

// // INS // //
in vec3 worldSpace_vertexPosition;
in vec3 worldSpace_vertexNormal;

in vec3 camSpace_vertexPosition;
in vec3 camSpace_camDirection;
in vec3 camSpace_lightDirection[NUM_LIGHTS];
in vec3 camSpace_normalDirection;

in vec3 tangentSpace_camPosition;
in vec3 tangentSpace_lightPosition[NUM_LIGHTS];
in vec3 tangentSpace_fragPosition;

in vec2 uv;

// // UNIFORMS // //
struct Material
{
	sampler2D diffuse;
	vec3 specular;
	float spec_power;
};
uniform Material material;

struct Point_Light
{
	vec3 position;
	float brightness;
	vec3 color;
};
uniform Point_Light point_light[NUM_LIGHTS];

struct Camera
{
	vec3 position;
};


uniform Camera camera;

// // OUTS // //
out vec4 fragColor;

// // LOCALS // //
vec3 worldSpace_camDirection;
vec3 worldSpace_lightDirection[NUM_LIGHTS];
vec3 worldSpace_normalDirection;

vec3 camSpace_camNormalized;
vec3 camSpace_lightNormalized[NUM_LIGHTS];
vec3 camSpace_normalNormalized;

vec3 tangentSpace_camDirection;
vec3 tangentSpace_lightDirection[NUM_LIGHTS];
vec3 tangentSpace_normal;


// INITIALIZATION OF LOCALS //
void init_worldSpace()
{
	worldSpace_camDirection = normalize(camera.position - worldSpace_vertexPosition);
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		worldSpace_lightDirection[i] = normalize(point_light[i].position - worldSpace_vertexPosition);
	}
	worldSpace_normalDirection = normalize(worldSpace_vertexNormal);
}

void init_camSpace()
{
	camSpace_camNormalized = normalize(camSpace_camDirection);
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		camSpace_lightNormalized[i] = normalize(camSpace_lightDirection[i]);
	}
	camSpace_normalNormalized = normalize(camSpace_normalDirection);
}

void init_tangentSpace()
{
	tangentSpace_camDirection = normalize(tangentSpace_camPosition - tangentSpace_fragPosition);
	for (int i = 0; i < NUM_LIGHTS; i++) 
	{
		tangentSpace_lightDirection[i] = normalize(tangentSpace_lightPosition[i] - tangentSpace_fragPosition);
	}
	tangentSpace_normal = vec3(0.0, 0.0, 1.0);
	
}


// // UTILITY FUNCTIONS // //
bool vec_is_zero(vec3 v)
{
	return !(v.x > 0.0 && v.y > 0.0 && v.z > 0.0);
}

float dot_vec3(vec3 v1, vec3 v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

float attenuation(int index)
{
	float light_distance = distance(worldSpace_vertexPosition, point_light[index].position);
	return 1 / (light_distance * light_distance);
}

// // DIFFUSE LIGHTING // //
vec3 diffuse_lighting(vec3 normal, vec3 light)
{
	float cos_theta = clamp(dot(normal, light), 0, 1);
	return vec3(cos_theta);
}

vec3 diffuse_lighting_worldSpace(int index)
{
	return diffuse_lighting(
		worldSpace_normalDirection,
		worldSpace_lightDirection[index]);
}

vec3 diffuse_lighting_camSpace(int index)
{
	return diffuse_lighting(
		camSpace_normalNormalized, 
		camSpace_lightNormalized[index]);
}

vec3 diffuse_lighting_tangentSpace(int index)
{
	return diffuse_lighting(
		tangentSpace_normal, 
		tangentSpace_lightDirection[index]);
}

// // SPECULAR LIGHTING // //
vec3 specular_lighting(vec3 light, vec3 normal, vec3 cam)
{
	vec3 reflection = reflect(-light, normal);
	float cos_alpha = clamp( dot_vec3( cam, reflection ), 0, 1);
	return vec3(pow(cos_alpha, material.spec_power));
}

vec3 specular_lighting_worldSpace(int index)
{
	return specular_lighting(
		worldSpace_lightDirection[index], 
		worldSpace_normalDirection, 
		worldSpace_camDirection);
}

vec3 specular_lighting_camSpace(int index)
{
	return specular_lighting(
		camSpace_lightNormalized[index],
		camSpace_normalNormalized,
		camSpace_camNormalized);
}

vec3 specular_lighting_tangentSpace(int index)
{
	return specular_lighting(
		tangentSpace_lightDirection[index], 
		tangentSpace_normal, 
		tangentSpace_camDirection);
}

// // MAIN // //
void main ()
{
	vec3 diffuse_out = vec3(0.0);
	vec3 specular_out = vec3(0.0);

	init_worldSpace();
	init_camSpace();
	init_tangentSpace();
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		float temp_attenuation = attenuation(i);

		diffuse_out += 
			diffuse_lighting_worldSpace(i) *
			point_light[i].brightness * point_light[i].brightness *
			point_light[i].color * 
			temp_attenuation;

		specular_out +=
			specular_lighting_tangentSpace(i) * 
			material.specular * 
			point_light[i].color;
	}


	vec3 outColor = 
		diffuse_out * texture(material.diffuse, uv).rgb
		+ 
		specular_out
		* vec3(1.0);

	fragColor = vec4(outColor.xyz, 1.0);
}
