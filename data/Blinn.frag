#version 440 core
#pragma optionNV unroll all
#extension GL_EXT_texture_array : enable
#define NUM_LIGHTS 3
#define MAX_BLINN_SPECULAR_POWER 8
#define MAX_PHONG_SPECULAR_POWER 150

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

in vec2 flat_uv;

in vec3 test;

// // ----- UNIFORMS ----- // //
struct Material
{
	sampler2D diffuse;
	float diffuse_amount;
	
	sampler2D specular;
	float specular_amount;

	sampler2D glossiness;

	sampler2D normal;
	bool normal_directx_mode;

	sampler2D displacement;
	float displacement_amount;
	bool displacement_enabled;
};
uniform Material material;

struct PointLight
{
	vec3 position;
	float brightness;
	vec3 color;
	samplerCube depth;
	float far_plane;
	bool shadow_enabled;
};
uniform PointLight pointLight[NUM_LIGHTS];

struct DirectionalLight
{
	vec3 direction;
	float brightness;
	vec3 color;
	sampler2DArray depth;
	mat4 projection;
	bool shadow_enabled;
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
	bool shadow_enabled;
};
uniform SpotLight spotLight[NUM_LIGHTS];

struct Camera
{
	vec3 position;
};
uniform Camera camera;

// // ----- OUTS ----- // //
out layout(location = 0) vec4 frag_color;

// // ----- LOCAL FRAGMENT DIRECTIONS ----- // //
vec2 uv;

struct m_Material
{
	float specular_power;
} m_material;

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


// // ----- UTILITY STRUCTS ----- // //
struct Layers
{
	float num;
	float layer_depth;
	float current_depth;
};

struct ParallaxUVs
{
	vec2 current;
	vec2 prev;
	vec2 delta;
};

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
void init_tangent_space_cam();
void init_tangent_space_lights();
void init_tangent_space_normals();

void init_uv();
void calculate_displaced_uv();
Layers init_displaced_uv_layers();
ParallaxUVs init_parallax_uv_values(Layers);
float find_current_depth_layer(inout Layers, inout ParallaxUVs);
vec2 interpolate_depth(float, Layers, ParallaxUVs);

void init()
{
	init_tangent_space_cam();
	init_tangent_space_lights();
	init_uv();
	init_tangent_space_normals();
}

void init_tangent_space_cam()
{
	m_cam.tangent_space_direction = normalize(in_cam.tangent_space_position - in_frag.tangent_space_position);
}

void init_tangent_space_lights()
{
	for (int i = 0; i < NUM_LIGHTS; i++) 
	{
		m_pointLight[i].tangent_space_direction = normalize(in_pointLight[i].tangent_space_position - in_frag.tangent_space_position);
		m_directionalLight[i].tangent_space_direction = -in_directionalLight[i].tangent_space_position;
		m_spotLight[i].tangent_space_direction = normalize(in_spotLight[i].tangent_space_position - in_frag.tangent_space_position);
		m_spotLight[i].tangent_space_intensity = spot_intensity(
			in_frag.world_space_position,
			spotLight[i].position, 
			spotLight[i].direction, 
			spotLight[i].inner,
			spotLight[i].outer);
	}
}

void init_tangent_space_normals()
{
	m_frag.tangent_space_normal = texture(material.normal, uv).rgb;
	// Flip green channel for directx mode normal map
	if (material.normal_directx_mode)
	{
		m_frag.tangent_space_normal.g = 1 - m_frag.tangent_space_normal.g;
	}
	m_frag.tangent_space_normal = normalize(m_frag.tangent_space_normal * 2.0 - 1.0);
}
	

void init_uv()
{
	uv = flat_uv;
	if (material.displacement_enabled == true)
	{
		calculate_displaced_uv();
	}
}

void calculate_displaced_uv()
{
	Layers layers = init_displaced_uv_layers();
	ParallaxUVs uvs = init_parallax_uv_values(layers);

	float current_depth_map_value = find_current_depth_layer(layers, uvs);
	uv = interpolate_depth(current_depth_map_value, layers, uvs);
}

Layers init_displaced_uv_layers()
{
	Layers out_layers;
	out_layers.num = 10;
	out_layers.layer_depth = 1.0 / out_layers.num;
	out_layers.current_depth = 0.0;
	return out_layers;
}

ParallaxUVs init_parallax_uv_values(Layers layers)
{
	vec2 difference = m_cam.tangent_space_direction.xy * material.displacement_amount;

	ParallaxUVs out_uvs;
	out_uvs.delta = difference / layers.num;
	out_uvs.current = flat_uv;
	out_uvs.prev = vec2(0.0);
	return out_uvs;
}

float find_current_depth_layer(inout Layers layers, inout ParallaxUVs uvs)
{
	float current_depth_map_value = texture(material.displacement, uvs.current).r;

	while(layers.current_depth < current_depth_map_value)
	{
		uvs.current -= uvs.delta;
		current_depth_map_value = texture(material.displacement, uvs.current).r;
		layers.current_depth += layers.layer_depth;
	}
	return current_depth_map_value;
}

vec2 interpolate_depth(float current_depth_map_value, Layers layers, ParallaxUVs uvs)
{
	uvs.prev = uvs.current + uvs.delta;

	float current_depth = current_depth_map_value - layers.current_depth;
	float prev_depth = texture(material.displacement, uvs.prev).r - layers.current_depth + layers.layer_depth;

	float weight = current_depth / (current_depth - prev_depth);
	return uvs.prev * weight + uvs.current * (1.0 - weight);
}	
	

// // ----- DIFFUSE LIGHTING ----- // //
vec3 diffuse(vec3 normal, vec3 light)
{
	float cos_theta = clamp(dot(normal, light), 0, 1);
	return vec3(cos_theta);
}

vec3 diffuse_point_tangent_space(int index)
{
	return diffuse(
		m_frag.tangent_space_normal, 
		m_pointLight[index].tangent_space_direction);
}

vec3 diffuse_directional_tangent_space(int index)
{
	return diffuse(
		m_frag.tangent_space_normal,
		m_directionalLight[index].tangent_space_direction);

}

vec3 diffuse_spot_tangent_space(int index)
{

	return m_spotLight[index].tangent_space_intensity * vec3(1.0) *
		diffuse( 
			m_frag.tangent_space_normal,
			m_spotLight[index].tangent_space_direction);
}

// // ----- SPECULAR LIGHTING ----- // //
vec3 blinn_phong(vec3 light_direction, vec3 normal_direction, vec3 cam_direction)
{
	float specular_power = texture(material.glossiness, uv).r * MAX_BLINN_SPECULAR_POWER;
	specular_power = specular_power * specular_power * specular_power + 2.0;
	// Energy conservation approximation is (specular_power + 8)/(8 * pi)
	float energy_conservation_factor = 0.0397436 * specular_power + 0.0856832;
	vec3 half_way_direction = normalize(light_direction + cam_direction);
	float cos_alpha = clamp( dot(normal_direction, half_way_direction), 0.0, 1.0);
	return vec3(pow(cos_alpha, specular_power)) * energy_conservation_factor;
}

vec3 specular(vec3 light_direction, vec3 normal_direction, vec3 cam_direction)
{
	return blinn_phong(light_direction, normal_direction, cam_direction);
}

vec3 specular_point_tangent_space(int index)
{
	return specular(
		m_pointLight[index].tangent_space_direction, 
		m_frag.tangent_space_normal, 
		m_cam.tangent_space_direction);
}

vec3 specular_directional_tangent_space(int index)
{
	return specular(
		m_directionalLight[index].tangent_space_direction,
		m_frag.tangent_space_normal,
		m_cam.tangent_space_direction);
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

    uv = flat_uv;
	init();
	   
	// Point lights
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		float temp_shadow = 010;
		if (pointLight[i].shadow_enabled)
		{
			temp_shadow = create_point_shadow(
				pointLight[i].position,
				pointLight[i].far_plane,
				pointLight[i].depth);
		}
		float temp_attenuation = attenuation(pointLight[i].position);

		diffuse_out += 
			diffuse_point_tangent_space(i) *
			pointLight[i].brightness * pointLight[i].brightness *
			pointLight[i].color * 
			temp_attenuation * 
			temp_shadow;

		specular_out +=
			specular_point_tangent_space(i) * 
			pointLight[i].brightness *  pointLight[i].brightness *
			texture(material.specular, uv).rgb * 
			pointLight[i].color * 
			temp_attenuation * 
			temp_shadow;
	}

	// Directional lights
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		float temp_shadow = 1.0;
		if (directionalLight[i].shadow_enabled)
		{
			temp_shadow = create_directional_shadow(
				in_directionalLight[i].light_space_position, 
				m_directionalLight[i].tangent_space_direction,
				directionalLight[i].depth);
		}

		diffuse_out += 
			diffuse_directional_tangent_space(i) *
			directionalLight[i].brightness *
			directionalLight[i].color * 
			temp_shadow;

		specular_out +=
			specular_directional_tangent_space(i) *
			directionalLight[i].brightness *
			texture(material.specular, uv).rgb * 
			directionalLight[i].color * 
			temp_shadow;
	}

	// Spot lights
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		float temp_shadow = 1.0;
		if(spotLight[i].shadow_enabled)
		{
			temp_shadow = create_directional_shadow(
				in_spotLight[i].light_space_position,
				m_spotLight[i].tangent_space_direction,
				spotLight[i].depth);
		}
		float temp_attenuation = attenuation(spotLight[i].position);


		diffuse_out +=
			diffuse_spot_tangent_space(i) *
			spotLight[i].brightness * spotLight[i].brightness *
			spotLight[i].color * 
			temp_attenuation * 
			temp_shadow;

		specular_out +=
			specular_spot_tangent_space(i) *
			pointLight[i].brightness * spotLight[i].brightness *
			texture(material.specular, uv).rgb * 
			spotLight[i].color * 
			temp_attenuation * 
			temp_shadow;
	}

	vec3 outColor = 
		diffuse_out * texture(material.diffuse, uv).rgb * material.diffuse_amount
		+ 
		specular_out * material.specular_amount
		* vec3(1.0);

	frag_color = vec4(outColor.xyz, 1.0);
}
