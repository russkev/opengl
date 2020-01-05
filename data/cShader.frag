#version 440 core
#define NUM_LIGHTS 3
#extension GL_EXT_texture_array : enable

// // ----- INS ----- // //
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

in PointLight_Space
{
	vec4 position[NUM_LIGHTS];
} pointLight_space;
	
in SpotLight_Space
{
	vec4 position[NUM_LIGHTS];
} spotLight_space;

in DirectionalLight_Space
{
	vec4 position[NUM_LIGHTS];
} directionalLight_space;

in vec2 uv;

// // ----- UNIFORMS ----- // //
struct Material
{
	sampler2D diffuse;
	
	vec3 specular;
	float spec_power;
};
uniform Material material;

struct PointLight
{
	vec3 position;
	float brightness;
	vec3 color;
	samplerCube depth;
	mat4 projection;
	float far_plane;
};
uniform PointLight pointLight[NUM_LIGHTS];

struct DirectionalLight
{
	vec3 direction;
	float brightness;
	vec3 color;
	sampler2DArray depth;
	mat4 projection;
};
uniform DirectionalLight directionalLight[NUM_LIGHTS];

struct SpotLight
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
uniform SpotLight spotLight[NUM_LIGHTS];

struct Camera
{
	vec3 position;
};
uniform Camera camera;

// // ----- OUTS ----- // //
out vec4 fragColor;

// // ----- LOCAL FRAGMENT DIRECTIONS ----- // //
struct Frag_WorldSpace
{
	vec3 cam;
	vec3 pointLight[NUM_LIGHTS];
	vec3 spotLight[NUM_LIGHTS];
	float spotLight_intensity[NUM_LIGHTS];
	vec3 normal;
} frag_worldSpace;

//struct Local_PointLight
//{
//	vec3 worldSpace;
//	vec3 camSpace;
//	vec3 
vec3 tangentSpace_cam_direction;
vec3 tangentSpace_pointLight_direction[NUM_LIGHTS];
vec3 tangentSpace_directionalLight_normalized[NUM_LIGHTS];
vec3 tangentSpace_spotLight_direction[NUM_LIGHTS];
float tangentSpace_spotLight_intensity[NUM_LIGHTS];
vec3 tangentSpace_normal;

// // ----- UTILITY METHODS ----- // //
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


// // ----- INITIALIZATION OF LOCALS ----- // //
void init_worldSpace()
{
	frag_worldSpace.cam = normalize(camera.position - worldSpace.vertex_position);
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		frag_worldSpace.pointLight[i] = normalize(pointLight[i].position - worldSpace.vertex_position);
		frag_worldSpace.spotLight[i] = normalize(spotLight[i].position - worldSpace.vertex_position);
		frag_worldSpace.spotLight_intensity[i] = spot_intensity(
			worldSpace.vertex_position,
			spotLight[i].position, 
			spotLight[i].direction, 
			spotLight[i].inner, 
			spotLight[i].outer);
	}
	frag_worldSpace.normal = normalize(worldSpace.vertex_normal);
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
			spotLight[i].position, 
			spotLight[i].direction, 
			spotLight[i].inner, 
			spotLight[i].outer);
	}
	tangentSpace_normal = vec3(0.0, 0.0, 1.0);
	
}

// // ----- DIFFUSE LIGHTING ----- // //
vec3 diffuse(vec3 normal, vec3 light)
{
	float cos_theta = clamp(dot(normal, light), 0, 1);
	return vec3(cos_theta);
}

vec3 diffuse_point_worldSpace(int index)
{
	return diffuse(
		frag_worldSpace.normal,
		frag_worldSpace.pointLight[index]);
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
		frag_worldSpace.normal,
		-directionalLight[index].direction);
}

vec3 diffuse_directional_tangentSpace(int index)
{
	return diffuse(
		tangentSpace_normal,
		tangentSpace_directionalLight_normalized[index]);

}

vec3 diffuse_spot_worldSpace(int index)
{
	return frag_worldSpace.spotLight[index] * 
	diffuse(
		frag_worldSpace.normal,
		frag_worldSpace.spotLight[index]);
}

vec3 diffuse_spot_tangentSpace(int index)
{
	return tangentSpace_spotLight_intensity[index] * 
		diffuse( 
			tangentSpace_normal,
			tangentSpace_spotLight_direction[index]);
}

// // ----- SPECULAR LIGHTING ----- // //
vec3 specular(vec3 light, vec3 normal, vec3 cam)
{
	vec3 reflection = reflect(-light, normal);
	float cos_alpha = clamp( dot_vec3( cam, reflection ), 0, 1);
	return vec3(pow(cos_alpha, material.spec_power));
}

vec3 specular_point_worldSpace(int index)
{
	return specular(
		frag_worldSpace.pointLight[index], 
		frag_worldSpace.normal, 
		frag_worldSpace.cam);
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
		-directionalLight[index].direction,
		frag_worldSpace.normal,
		frag_worldSpace.cam);
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
	return frag_worldSpace.spotLight[index] * 
	specular(
		frag_worldSpace.spotLight[index],
		frag_worldSpace.normal,
		frag_worldSpace.cam);
}

vec3 specular_spot_tangentSpace(int index)
{
	return tangentSpace_spotLight_intensity[index] * 
	specular(
		tangentSpace_spotLight_direction[index],
		tangentSpace_normal,
		tangentSpace_cam_direction);
}

// // ----- SHADOWS ----- // //
float create_directional_shadow(vec4 lightSpace_position, vec3 tangentSpace_lightDirection, sampler2DArray depth)
{
	float num_passes	= 1;
	float step_size		= 3 / num_passes;
	float texel_ratio	= 1.0;
	vec2 texelSize		= texel_ratio / vec2(1024, 1024) /*textureSize(depthMap, 0)*/;

	float out_shadow	= 0.0;
	float max_bias		= 0.002;
	float bias_ratio	= 0.2;
	float bias =  max(max_bias * (1.0 - dot(tangentSpace_normal, tangentSpace_lightDirection)), max_bias * bias_ratio);

	vec3 projection_coordinates = lightSpace_position.xyz / lightSpace_position.w;
	projection_coordinates = projection_coordinates * 0.5 + 0.5;

	float closest_depth = texture(depth, vec3(projection_coordinates.xy, 0)).r;
	float current_depth = projection_coordinates.z;

	for(float x = -1; x < 1; x+= step_size)
	{
		for (float y = -1; y < 1; y += step_size)
		{
			float pcfDepth = texture(depth, vec3(projection_coordinates.xy + vec2(x, y) * texelSize, 0)).r;
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

float create_point_shadow(vec3 light_pos, float light_far_plane, samplerCube depth)
{
	vec3 frag_to_light = worldSpace.vertex_position - light_pos;
	float closest_depth = texture(depth, frag_to_light).r * light_far_plane;
	float current_depth = length(frag_to_light);

	float bias = 0.2;
	return current_depth - bias > closest_depth ? 0.0 : 1.0;
}



float linearize_depth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * 0.1 * 1000) / (1000.1 - z * (1000 - 0.1));	
}	


// // ----- MAIN ----- // //
void main ()
{
	vec3 diffuse_out = vec3(0.0);
	vec3 specular_out = vec3(0.0);

	init_worldSpace();
	init_tangentSpace();


	// Point lights
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		float temp_shadow = create_point_shadow(
			pointLight[i].position,
			pointLight[i].far_plane,
			pointLight[i].depth);
		float temp_attenuation = attenuation(pointLight[i].position);

		diffuse_out += 
			diffuse_point_worldSpace(i) *
			pointLight[i].brightness * pointLight[i].brightness *
			pointLight[i].color * 
			temp_attenuation * 
			temp_shadow;

		specular_out +=
			specular_point_worldSpace(i) * 
			pointLight[i].brightness *  pointLight[i].brightness *
			material.specular * 
			pointLight[i].color * 
			temp_attenuation * 
			temp_shadow;
	}

	// Directional lights
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		float temp_shadow = create_directional_shadow(
			directionalLight_space.position[i], 
			tangentSpace_directionalLight_normalized[i],
			directionalLight[i].depth);

		diffuse_out += 
			diffuse_directional_worldSpace(i) *
			directionalLight[i].brightness *
			directionalLight[i].color * 
			temp_shadow;

		specular_out +=
			specular_directional_tangentSpace(i) *
			directionalLight[i].brightness *
			material.specular * 
			directionalLight[i].color * 
			temp_shadow;
	}

	// Spot lights
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		float temp_attenuation = attenuation(spotLight[i].position);
		float temp_shadow = create_directional_shadow(
			spotLight_space.position[i], 
			tangentSpace_spotLight_direction[i],
			spotLight[i].depth);

		diffuse_out +=
			diffuse_spot_tangentSpace(i) * 
			spotLight[i].brightness * spotLight[i].brightness *
			spotLight[i].color * 
			temp_attenuation * 
			temp_shadow;
		specular_out +=
			specular_spot_tangentSpace(i) *
			pointLight[i].brightness * spotLight[i].brightness *
			material.specular * 
			spotLight[i].color * 
			temp_attenuation * 
			temp_shadow;
	}

	vec3 outColor = 
		diffuse_out * texture(material.diffuse, uv).rgb
		+ 
		specular_out
//		temp_shadow
		* vec3(1.0);

	fragColor = vec4(outColor.xyz, 1.0);
}
