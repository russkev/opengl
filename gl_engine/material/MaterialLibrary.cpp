#include "MaterialLibrary.h"


#include "../node/CameraNode.h"
#include "../node/Node.h"

namespace gl_engine
{
	// BLINN
	//------------------------------------------------------------------------------------------------------------------------------------------//

	BlinnMaterial::BlinnMaterial(const std::string& name) :
		Material{ name, "Blinn.vert", "Blinn.frag"}
	{
		init();
	}

	void BlinnMaterial::update_view(CameraNode* cameraNode, Node* node)
	{
		set_uniform(k_transform_model_to_projection, cameraNode->world_to_projection() * node->world_to_node());
		set_uniform(k_transform_model_to_world, node->world_to_node());
		set_uniform(k_transform_model_world_to_normal, node->world_normal_to_node());
		set_uniform(k_camera_position, cameraNode->world_position());
	}

	void BlinnMaterial::init()
	{
		set_uniform(k_transform_model_to_projection,	glm::mat4{ 1.0f });
		set_uniform(k_transform_model_to_world,			glm::mat4{ 1.0f });
		//set_uniform(k_transform_model_world_to_normal,	glm::mat3{ 1.0f });

		for (GLuint i = 0; i < k_num_point_lights; ++i)
		{
			std::string index = std::to_string(i);
			set_uniform(k_point_light + "[" + index + "]." + k_position,	glm::vec3{ 0.0f });
			set_uniform(k_point_light + "[" + index + "]." + k_brightness,	0.0f);
			set_uniform(k_point_light + "[" + index + "]." + k_color,		glm::vec3{ 1.0f });
			set_sampler_value(k_point_light + "[" + index + "]." + k_depth,	0.0f);
			set_uniform(k_point_light + "[" + index + "]." + k_projection,	glm::mat4{ 1.0f });
			set_uniform(k_point_light + "[" + index + "]." + k_far_plane,	100.0f);
		}
		//

		for (GLuint i = 0; i < k_num_directional_lights; ++i)
		{
			std::string index = std::to_string(i);
			set_uniform(k_directional_light + "[" + index + "]." + k_direction,		glm::vec3{ 0.0f });
			set_uniform(k_directional_light + "[" + index + "]." + k_brightness,	0.0f);
			set_uniform(k_directional_light + "[" + index + "]." + k_color,			glm::vec3{ 1.0f });
			set_sampler_value(k_directional_light + "[" + index + "]." + k_depth,	0.0f);
			set_uniform(k_directional_light + "[" + index + "]." + k_projection,	glm::mat4{ 1.0f });
		}

		for (GLuint i = 0; i < k_num_spot_lights; ++i)
		{
			std::string index = std::to_string(i);
			set_uniform(k_spot_light + "[" + index + "]." + k_position,		glm::vec3{ 0.0f });
			set_uniform(k_spot_light + "[" + index + "]." + k_direction,	glm::vec3{ 0.0f });
			set_uniform(k_spot_light + "[" + index + "]." + k_brightness,	0.0f);
			set_uniform(k_spot_light + "[" + index + "]." + k_color,		glm::vec3{1.0f});
			set_uniform(k_spot_light + "[" + index + "]." + k_inner,		30.0f);
			set_uniform(k_spot_light + "[" + index + "]." + k_outer,		40.0f);
			set_sampler_value(k_spot_light + "[" + index + "]." + k_depth,	0.0f);
			set_uniform(k_spot_light + "[" + index + "]." + k_projection,	glm::mat4{ 1.0f });
		}

		
		set_uniform(k_camera_position, glm::vec3{ 0.0f });
		
		set_sampler_value(k_material_diffuse,			0.5f);
		set_uniform(k_material_diffuse_amount,			1.0f);
		set_sampler_value(k_material_specular,			0.0f);
		set_uniform(k_material_specular_amount,			0.0f);
		set_sampler_value(k_material_glossiness,		0.5f);
		set_sampler_color(k_material_normal,			glm::vec3{0.5f, 0.5f, 1.0f});
		set_uniform(k_material_normal_directx_mode,		false);
		set_sampler_value(k_material_displacement,		0.0f);
		set_uniform(k_material_displacement_amount,		0.1f);
		set_uniform(k_material_displacement_enabled,	false);
	}

	// LIGHT
	//------------------------------------------------------------------------------------------------------------------------------------------//

	LightMaterial::LightMaterial(const std::string& name) :
		Material{name, "Light.vert", "Light.frag"}
	{
		init();
	}

	void LightMaterial::update_view(CameraNode* cameraNode, Node* node)
	{
		set_uniform(k_transform_model_to_projection, cameraNode->world_to_projection() * node->world_to_node());
	}

	void LightMaterial::init()
	{
		set_uniform(k_transform_model_to_projection, glm::mat4{ 1.0f });
		set_uniform(k_light_position, glm::vec3{ 0.0f });
		set_uniform(k_light_brightness, 0.0f);
		set_uniform(k_light_color, glm::vec3{ 0.0f });
	}
} // namespace gl_engine