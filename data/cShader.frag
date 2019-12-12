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
vec3 camSpace_camNormalized;
vec3 camSpace_lightNormalized[NUM_LIGHTS];
vec3 camSpace_normalNormalized;

vec3 tangentSpace_camDirection;
vec3 tangentSpace_lightDirection[NUM_LIGHTS];
vec3 tangentSpace_normal;


// INITIALIZATION OF LOCALS //
void init_camSpace()
{
	camSpace_camNormalized = normalize(camSpace_camDirection);
	for (int i = 0; i < 5; i++)
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
//	return 1;
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
		worldSpace_vertexNormal, 
		normalize(point_light[index].position));
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
	vec3 light		= normalize(point_light[index].position - worldSpace_vertexPosition);
	vec3 normal		= normalize(worldSpace_vertexNormal);
	vec3 cam		= normalize(camera.position - worldSpace_vertexPosition);
	return specular_lighting(light, normal, cam);
}

//vec3 specular_lighting_camSpace()
//{
//	return specular_lighting(
//		camSpace_lightNormalized,
//		camSpace_normalNormalized,
//		camSpace_camNormalized);
//}

//vec3 specular_lighting_tangentSpace()
//{
//	return specular_lighting(
//		tangentSpace_lightDirection, 
//		tangentSpace_normal, 
//		tangentSpace_camDirection);
//}

// // MAIN // //
void main ()
{

	float out_brightness	= point_light[0].brightness * point_light[0].brightness;
//	float light_attenuation	= attenuation();

	vec3 diffuse_out = vec3(0.0);
	vec3 specular_out = vec3(0.0);

	init_camSpace();
	init_tangentSpace();
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		float temp_attenuation = attenuation(i);
		diffuse_out += 
			diffuse_lighting_worldSpace(i) * //(worldSpace_vertexNormal, normalize(point_light[i].position)) *
			point_light[i].brightness * point_light[i].brightness *
			point_light[i].color * 
			temp_attenuation;
		specular_out +=
			specular_lighting_worldSpace(i) * 
			material.specular * 
			point_light[i].color;



//		diffuse_out += diffuse_lighting_worldSpace(i);
//		specular_lighting += specular_lighting_worldSpace(i);
	}

	//vec3 diffuse_lighting = diffuse_lighting_camSpace();
	//vec3 specular_lighting = specular_lighting_camSpace();

	vec3 outColor = 
		diffuse_out * texture(material.diffuse, uv).rgb
//		diffuse_out * texture(material.diffuse, uv).rgb * out_brightness * point_light[0].color
		+ 
		specular_out
		//specular_lighting * material.specular * point_light.color
		//diffuse_lighting * texture(material.diffuse, uv).rgb * out_brightness * point_light[0].color.xyz
		* vec3(1.0);

	fragColor = vec4(outColor.xyz, 1.0);
}
