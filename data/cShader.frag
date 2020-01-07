#version 440 core
#pragma optionNV unroll all
#extension GL_EXT_texture_array : enable
#define NUM_LIGHTS 3

// // ----- INS ----- // //
in Out_Frag
{
	vec3 world_space_position;
	vec3 world_space_normal;
	vec3 tangent_space_position;
} in_frag;

in Out_Cam
{
	vec3 tangent_space_position;
} in_cam;

in Out_PointLight
{
	vec3 tangent_space_position;
} in_pointLight[NUM_LIGHTS];

in Out_SpotLight
{
	vec4 light_space_position;
	vec3 tangent_space_position;
} in_spotLight[NUM_LIGHTS];

in Out_DirectionalLight
{
	vec4 light_space_position;
	vec3 tangent_space_position;
} in_directionalLight[NUM_LIGHTS];

in vec2 uv;

in vec3 test;

// // ----- UNIFORMS ----- // //
uniform bool is_blinn;

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
struct m_Frag
{
	vec3 world_space_normal;
	vec3 tangent_space_normal;
} m_frag;

struct m_Cam
{
	vec3 world_space_direction;
	vec3 tangent_space_direction;
} m_cam;

struct m_PointLight
{
	vec3 world_space_direction;
	vec3 tangent_space_direction;
} m_pointLight[NUM_LIGHTS];

struct m_DirectionalLight
{
	vec3 tangent_space_direction;
} m_directionalLight[NUM_LIGHTS];

struct m_SpotLight
{
	vec3 world_space_direction;
	vec3 tangent_space_direction;
	float world_space_intensity;
	float tangent_space_intensity;
} m_spotLight[NUM_LIGHTS];

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
	float light_distance = distance(in_frag.world_space_position, position);
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
void init_world_space()
{
	m_cam.world_space_direction = normalize(camera.position - in_frag.world_space_position);
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		m_pointLight[i].world_space_direction = normalize(pointLight[i].position - in_frag.world_space_position);
		m_spotLight[i].world_space_direction = normalize(spotLight[i].position - in_frag.world_space_position);
		m_spotLight[i].world_space_intensity = spot_intensity(
			in_frag.world_space_position,
			spotLight[i].position, 
			spotLight[i].direction, 
			spotLight[i].inner, 
			spotLight[i].outer);
	}
	m_frag.world_space_normal = normalize(in_frag.world_space_normal);
}

void init_tangent_space()
{
	m_cam.tangent_space_direction = normalize(in_cam.tangent_space_position - in_frag.tangent_space_position);
	for (int i = 0; i < NUM_LIGHTS; i++) 
	{
		m_pointLight[i].tangent_space_direction = normalize(in_pointLight[i].tangent_space_position - in_frag.tangent_space_position);
		m_directionalLight[i].tangent_space_direction = -in_directionalLight[i].tangent_space_position;
//		m_directionalLight[i].tangent_space_direction = -normalize(in_directionalLight[i].tangent_space_position);
		m_spotLight[i].tangent_space_direction = normalize(in_spotLight[i].tangent_space_position - in_frag.tangent_space_position);
		m_spotLight[i].tangent_space_intensity = spot_intensity(
			in_frag.world_space_position,
			spotLight[i].position, 
			spotLight[i].direction, 
			spotLight[i].inner, 
			spotLight[i].outer);
	}
	m_frag.tangent_space_normal = vec3(0.0, 0.0, 1.0);
	
}

// // ----- DIFFUSE LIGHTING ----- // //
vec3 diffuse(vec3 normal, vec3 light)
{
	float cos_theta = clamp(dot(normal, light), 0, 1);
	return vec3(cos_theta);
}

vec3 diffuse_point_world_space(int index)
{
	return diffuse(
		m_frag.world_space_normal,
		m_pointLight[index].world_space_direction);
}

vec3 diffuse_point_tangent_space(int index)
{
	return diffuse(
		m_frag.tangent_space_normal, 
		m_pointLight[index].tangent_space_direction);
}

vec3 diffuse_directional_world_space(int index)
{
	return diffuse(
		m_frag.world_space_normal,
		-directionalLight[index].direction);
}

vec3 diffuse_directional_tangent_space(int index)
{
	return diffuse(
		m_frag.tangent_space_normal,
		m_directionalLight[index].tangent_space_direction);

}

vec3 diffuse_spot_world_space(int index)
{
	return m_spotLight[index].world_space_direction * 
	diffuse(
		m_frag.world_space_normal,
		m_spotLight[index].world_space_direction);
}

vec3 diffuse_spot_tangent_space(int index)
{

	return m_spotLight[index].tangent_space_intensity * vec3(1.0) *
		diffuse( 
			m_frag.tangent_space_normal,
			m_spotLight[index].tangent_space_direction);
}

// // ----- SPECULAR LIGHTING ----- // //


vec3 phong(vec3 light_direction, vec3 normal_direction, vec3 cam_direction)
{
	// Energy conservation approximation is (spec_power + 2)/(2 * pi)
	float energy_conservation_factor = 0.159 * material.spec_power + 0.318;
	vec3 reflection_direction = reflect(-light_direction, normal_direction);
	float cos_alpha = clamp( dot_vec3( cam_direction, reflection_direction ), 0, 1);
	return vec3(pow(cos_alpha, material.spec_power)) * energy_conservation_factor;

}

vec3 blinn_phong(vec3 light_direction, vec3 normal_direction, vec3 cam_direction)
{
	// Energy conservation approximation is (spec_power + 8)/(8 * pi)
	float energy_conservation_factor = 0.0397436 * material.spec_power + 0.0856832;
	vec3 half_way_direction = normalize(light_direction + cam_direction);
	float cos_alpha = clamp( dot(normal_direction, half_way_direction), 0.0, 1.0);
	return vec3(pow(cos_alpha, material.spec_power)) * energy_conservation_factor;
}

vec3 specular(vec3 light_direction, vec3 normal_direction, vec3 cam_direction)
{
	if (is_blinn)
	{
		return blinn_phong(light_direction, normal_direction, cam_direction);
	}
	else
	{
		return phong(light_direction, normal_direction, cam_direction);
	}
}

vec3 specular_point_world_space(int index)
{
	return specular(
		m_pointLight[index].world_space_direction, 
		m_frag.world_space_normal, 
		m_cam.world_space_direction);
}

vec3 specular_point_tangent_space(int index)
{
	return specular(
		m_pointLight[index].tangent_space_direction, 
		m_frag.tangent_space_normal, 
		m_cam.tangent_space_direction);
}

vec3 specular_directional_world_space(int index)
{
	return specular(
		-directionalLight[index].direction,
		m_frag.world_space_normal,
		m_cam.world_space_direction);
}

vec3 specular_directional_tangent_space(int index)
{
	return specular(
		m_directionalLight[index].tangent_space_direction,
		m_frag.tangent_space_normal,
		m_cam.tangent_space_direction);
}

vec3 specular_spot_world_space(int index)
{
	return m_spotLight[index].world_space_direction * 
	specular(
		m_spotLight[index].world_space_direction,
		m_frag.world_space_normal,
		m_cam.world_space_direction);
}

vec3 specular_spot_tangent_space(int index)
{
	return m_spotLight[index].tangent_space_intensity * 
	specular(
		m_spotLight[index].tangent_space_direction,
		m_frag.tangent_space_normal,
		m_cam.tangent_space_direction);
}

// // ----- SHADOWS ----- // //
float create_directional_shadow(vec4 lightSpace_position, vec3 tangent_space_lightDirection, sampler2DArray depth)
{
	float num_passes	= 1;
	float step_size		= 3 / num_passes;
	float texel_ratio	= 1.0;
	vec2 texelSize		= texel_ratio / vec2(1024, 1024) /*!!!textureSize(depthMap, 0)*/;

	float out_shadow	= 0.0;
	float max_bias		= 0.002;
	float bias_ratio	= 0.2;
	float bias =  max(max_bias * (1.0 - dot(m_frag.tangent_space_normal, tangent_space_lightDirection)), max_bias * bias_ratio);

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
	float out_shadow = 1;
	float bias = 0.05;
	int samples = 20;
	float disk_radius = 0.05;
	vec3 frag_to_light = in_frag.world_space_position - light_pos;
	float current_depth = length(frag_to_light);

	vec3 sample_offset_directions[20] = vec3[]
	(
		vec3(+1, +1, +1), vec3(+1, -1, +1), vec3(-1, -1, +1), vec3(-1, +1, +1),
		vec3(+1, +1, -1), vec3(+1, -1, -1), vec3(-1, -1, -1), vec3(-1, +1, -1),
		vec3(+1, +1, +0), vec3(+1, -1, +0), vec3(-1, -1, +0), vec3(-1, +1, +0),
		vec3(+1, +0, +1), vec3(-1, +0, +1), vec3(+1, +0, -1), vec3(-1, +0, -1),
		vec3(+0, +1, +1), vec3(+0, -1, +1), vec3(+0, -1, -1), vec3(+0, +1, -1)
	);

	for (int i = 0; i < samples; ++i)
	{
		float closest_depth = 
			texture(depth, frag_to_light + sample_offset_directions[i] * disk_radius).r * light_far_plane;
		out_shadow += current_depth - bias > closest_depth ? 0.0 : 1.0;
	}
	out_shadow /= float(samples);


	return out_shadow;

//	return current_depth - bias > closest_depth ? 0.0 : 1.0;
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

	init_world_space();
	init_tangent_space();


	// Point lights
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		float temp_shadow = create_point_shadow(
			pointLight[i].position,
			pointLight[i].far_plane,
			pointLight[i].depth);
		float temp_attenuation = attenuation(pointLight[i].position);

		diffuse_out += 
			diffuse_point_world_space(i) *
			pointLight[i].brightness * pointLight[i].brightness *
			pointLight[i].color * 
			temp_attenuation * 
			temp_shadow;

		specular_out +=
			specular_point_tangent_space(i) * 
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
			in_directionalLight[i].light_space_position, 
			m_directionalLight[i].tangent_space_direction,
			directionalLight[i].depth);

		diffuse_out += 
			diffuse_directional_world_space(i) *
			directionalLight[i].brightness *
			directionalLight[i].color * 
			temp_shadow;

		specular_out +=
			specular_directional_tangent_space(i) *
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
			in_spotLight[i].light_space_position,
			m_spotLight[i].tangent_space_direction,
			spotLight[i].depth);

		diffuse_out +=
			diffuse_spot_tangent_space(i) *
			spotLight[i].brightness * spotLight[i].brightness *
			spotLight[i].color * 
			temp_attenuation * 
			temp_shadow;

		specular_out +=
			specular_spot_tangent_space(i) *
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
//		test
		* vec3(1.0);

	fragColor = vec4(outColor.xyz, 1.0);
}
