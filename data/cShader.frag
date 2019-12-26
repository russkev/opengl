#version 440 core
#define NUM_LIGHTS 3
#extension GL_EXT_texture_array : enable

// // INS // //
in WorldSpace
{
	vec3 vertex_position;
	vec3 vertex_normal;
} worldSpace;

in CamSpace
{
	vec3 vertex_position;
	vec3 cam_direction;
	vec3 light_direction[NUM_LIGHTS];
	vec3 normal_direction;
} camSpace;

in TangentSpace
{
	vec3 cam_position;
	vec3 pointLight_position[NUM_LIGHTS];
	vec3 directionalLight_direction[NUM_LIGHTS];
	vec3 spotLight_position[NUM_LIGHTS];
	vec3 frag_position;
} tangentSpace;
	
in LightSpace
{
	vec4 spotLight_position[NUM_LIGHTS];
} lightSpace;

in vec2 uv;

// // UNIFORMS // //
struct Material
{
	sampler2D diffuse;
	
	vec3 specular;
	float spec_power;
};
uniform Material material;

uniform sampler2DArray depthMap;
uniform sampler2D test_sampler;

struct Point_Light
{
	vec3 position;
	float brightness;
	vec3 color;
};
uniform Point_Light point_light[NUM_LIGHTS];

struct Directional_Light
{
	vec3 direction;
	float brightness;
	vec3 color;
};
uniform Directional_Light directional_light[NUM_LIGHTS];

struct Spot_Light
{
	vec3 position;
	vec3 direction;
	float brightness;
	vec3 color;
	float inner;
	float outer;
	sampler2DArray depth;
	mat4 projection;
};
uniform Spot_Light spot_light[NUM_LIGHTS];

struct Camera
{
	vec3 position;
};


uniform Camera camera;

// // OUTS // //
out vec4 fragColor;

// // LOCALS // //
vec3 worldSpace_cam_direction;
vec3 worldSpace_pointLight_direction[NUM_LIGHTS];
vec3 worldSpace_spotLight_direction[NUM_LIGHTS];
float worldSpace_spotLight_intensity[NUM_LIGHTS];
vec3 worldSpace_normal_direction;

vec3 camSpace_cam_normalized;
vec3 camSpace_light_normalized[NUM_LIGHTS];
vec3 camSpace_normal_normalized;

vec3 tangentSpace_cam_direction;
vec3 tangentSpace_pointLight_direction[NUM_LIGHTS];
vec3 tangentSpace_directionalLight_normalized[NUM_LIGHTS];
vec3 tangentSpace_spotLight_direction[NUM_LIGHTS];
float tangentSpace_spotLight_intensity[NUM_LIGHTS];
vec3 tangentSpace_normal;

// // UTILITY FUNCTIONS // //
bool vec_is_zero(vec3 v)
{
	return !(v.x > 0.0 && v.y > 0.0 && v.z > 0.0);
}

float dot_vec3(vec3 v1, vec3 v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

float attenuation(vec3 position)
{
	float light_distance = distance(worldSpace.vertex_position, position);
	return 1 / (light_distance * light_distance);
}

float spot_intensity(vec3 vertex_position, vec3 light_position, vec3 light_aim, float inner, float outer)
{
	vec3 light_direction = normalize(vertex_position - light_position);
	float cos_angle = dot(light_aim, light_direction);

	float intensity = clamp((cos_angle - outer) / (inner - outer), 0.0, 1.0);

	return intensity;
}


// INITIALIZATION OF LOCALS //
void init_worldSpace()
{
	worldSpace_cam_direction = normalize(camera.position - worldSpace.vertex_position);
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		worldSpace_pointLight_direction[i] = normalize(point_light[i].position - worldSpace.vertex_position);
		worldSpace_spotLight_direction[i] = normalize(spot_light[i].position - worldSpace.vertex_position);
		worldSpace_spotLight_intensity[i] = spot_intensity(
			worldSpace.vertex_position,
			spot_light[i].position, 
			spot_light[i].direction, 
			spot_light[i].inner, 
			spot_light[i].outer);
	}
	worldSpace_normal_direction = normalize(worldSpace.vertex_normal);
}

void init_camSpace()
{
	camSpace_cam_normalized = normalize(camSpace.cam_direction);
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		camSpace_light_normalized[i] = normalize(camSpace.light_direction[i]);
	}
	camSpace_normal_normalized = normalize(camSpace.normal_direction);
}

void init_tangentSpace()
{
	tangentSpace_cam_direction = normalize(tangentSpace.cam_position - tangentSpace.frag_position);
	for (int i = 0; i < NUM_LIGHTS; i++) 
	{
		tangentSpace_pointLight_direction[i] = normalize(tangentSpace.pointLight_position[i] - tangentSpace.frag_position);
		tangentSpace_directionalLight_normalized[i] = -normalize(tangentSpace.directionalLight_direction[i]);
		tangentSpace_spotLight_direction[i] = normalize(tangentSpace.spotLight_position[i] - tangentSpace.frag_position);
		tangentSpace_spotLight_intensity[i] = spot_intensity(
			worldSpace.vertex_position,
			spot_light[i].position, 
			spot_light[i].direction, 
			spot_light[i].inner, 
			spot_light[i].outer);
	}
	tangentSpace_normal = vec3(0.0, 0.0, 1.0);
	
}

// // DIFFUSE LIGHTING // //
vec3 diffuse(vec3 normal, vec3 light)
{
	float cos_theta = clamp(dot(normal, light), 0, 1);
	return vec3(cos_theta);
}

vec3 diffuse_point_worldSpace(int index)
{
	return diffuse(
		worldSpace_normal_direction,
		worldSpace_pointLight_direction[index]);
}

vec3 diffuse_point_camSpace(int index)
{
	return diffuse(
		camSpace_normal_normalized, 
		camSpace_light_normalized[index]);
}

vec3 diffuse_point_tangentSpace(int index)
{
	return diffuse(
		tangentSpace_normal, 
		tangentSpace_pointLight_direction[index]);
}

vec3 diffuse_directional_worldSpace(int index)
{
	return diffuse(
		worldSpace_normal_direction,
		-directional_light[index].direction);
}

vec3 diffuse_directional_tangentSpace(int index)
{
	return diffuse(
		tangentSpace_normal,
		tangentSpace_directionalLight_normalized[index]);

}

vec3 diffuse_spot_worldSpace(int index)
{
	return worldSpace_spotLight_intensity[index] * 
	diffuse(
		worldSpace_normal_direction,
		worldSpace_spotLight_direction[index]);
}

vec3 diffuse_spot_tangentSpace(int index)
{
	return tangentSpace_spotLight_intensity[index] * 
		diffuse( 
			tangentSpace_normal,
			tangentSpace_spotLight_direction[index]);
}

// // SPECULAR LIGHTING // //
vec3 specular(vec3 light, vec3 normal, vec3 cam)
{
	vec3 reflection = reflect(-light, normal);
	float cos_alpha = clamp( dot_vec3( cam, reflection ), 0, 1);
	return vec3(pow(cos_alpha, material.spec_power));
}

vec3 specular_point_worldSpace(int index)
{
	return specular(
		worldSpace_pointLight_direction[index], 
		worldSpace_normal_direction, 
		worldSpace_cam_direction);
}

vec3 specular_point_camSpace(int index)
{
	return specular(
		camSpace_light_normalized[index],
		camSpace_normal_normalized,
		camSpace_cam_normalized);
}

vec3 specular_point_tangentSpace(int index)
{
	return specular(
		tangentSpace_pointLight_direction[index], 
		tangentSpace_normal, 
		tangentSpace_cam_direction);
}

vec3 specular_directional_worldSpace(int index)
{
	return specular(
		-directional_light[index].direction,
		worldSpace_normal_direction,
		worldSpace_cam_direction);
}

vec3 specular_directional_tangentSpace(int index)
{
	return specular(
		tangentSpace_directionalLight_normalized[index],
		tangentSpace_normal,
		tangentSpace_cam_direction);
}

vec3 specular_spot_worldSpace(int index)
{
	return worldSpace_spotLight_intensity[index] * 
	specular(
		worldSpace_spotLight_direction[index],
		worldSpace_normal_direction,
		worldSpace_cam_direction);
}

vec3 specular_spot_tangentSpace(int index)
{
	return tangentSpace_spotLight_intensity[index] * 
	specular(
		tangentSpace_spotLight_direction[index],
		tangentSpace_normal,
		tangentSpace_cam_direction);
}

// // SHADOW // //
float create_shadow(int index)
{
	float num_passes	= 1;
	float step_size		= 3 / num_passes;
	float texel_ratio	= 1.0;
	vec2 texelSize		= texel_ratio / vec2(1024, 1024) /*textureSize(depthMap, 0)*/;

	float out_shadow	= 0.0;
	float max_bias		= 0.002;
	float bias_ratio	= 0.2;
	float bias =  max(max_bias * (1.0 - dot(tangentSpace_normal, tangentSpace_spotLight_direction[index])), max_bias * bias_ratio);

	vec3 projection_coordinates = lightSpace.spotLight_position[index].xyz / lightSpace.spotLight_position[index].w;
	projection_coordinates = projection_coordinates * 0.5 + 0.5;

	float closest_depth = texture(depthMap, vec3(projection_coordinates.xy, 0)).r;
	float current_depth = projection_coordinates.z;

	for(float x = -1; x < 1; x+= step_size)
	{
		for (float y = -1; y < 1; y += step_size)
		{
			float pcfDepth = texture(depthMap, vec3(projection_coordinates.xy + vec2(x, y) * texelSize, 0)).r;
			out_shadow += current_depth - bias > pcfDepth ? 0.0 : 1.0;
		}
	}
	out_shadow /= (num_passes * num_passes);

	


	if (projection_coordinates.z > 1.0f)
	{
		out_shadow = 1.0;
	}
	return out_shadow;
}

float linearize_depth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * 0.1 * 1000) / (1000.1 - z * (1000 - 0.1));	
}	


// // MAIN // //
void main ()
{
	vec3 diffuse_out = vec3(0.0);
	vec3 specular_out = vec3(0.0);

	init_worldSpace();
	init_camSpace();
	init_tangentSpace();

	//float shadowMap_tex = clamp(create_shadow(), 0.0, 1.0);
	float shadowMap_tex = 1.0;


	// Point lights
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		float temp_attenuation = attenuation(point_light[i].position);

		diffuse_out += 
			diffuse_point_worldSpace(i) *
			point_light[i].brightness * point_light[i].brightness *
			point_light[i].color * 
			temp_attenuation;

		specular_out +=
			specular_point_worldSpace(i) * 
			point_light[i].brightness *  point_light[i].brightness *
			material.specular * 
			point_light[i].color * 
			temp_attenuation;
	}

	// Directional lights
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		diffuse_out += 
			diffuse_directional_worldSpace(i) *
			directional_light[i].brightness *
			directional_light[i].color;

		specular_out +=
			specular_directional_tangentSpace(i) *
			directional_light[i].brightness *
			material.specular * 
			directional_light[i].color;
	}

	// Spot lights
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		float temp_attenuation = attenuation(spot_light[i].position);

		diffuse_out +=
			diffuse_spot_tangentSpace(i) * 
			spot_light[i].brightness * spot_light[i].brightness *
			spot_light[i].color * 
			temp_attenuation * 
			shadowMap_tex;
		specular_out +=
			specular_spot_tangentSpace(i) *
			point_light[i].brightness * spot_light[i].brightness *
			material.specular * 
			spot_light[i].color * 
			temp_attenuation * 
			shadowMap_tex;
	}

	//vec3 projection_coordinates = lightSpace.position.xyz / lightSpace.position.w;
	//projection_coordinates = projection_coordinates * 0.5 + 0.5;

	vec3 outColor = 
		diffuse_out * texture(material.diffuse, uv).rgb
		+ 
		specular_out
//		texture(depthMap, vec3(uv, 0)).rgb * 3
//		create_shadow()
		* vec3(1.0);

	fragColor = vec4(outColor.xyz, 1.0);
}
