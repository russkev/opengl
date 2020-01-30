#include "MaterialLibrary.h"

#include "../node/Node.h"
#include "../node/CameraNode.h"
#include "../node/LightNode.h"
#include "../light/PointLight.h"
#include "../light/DirectionalLight.h"
#include "../light/SpotLight.h"

namespace gl_engine
{
	// BLINN
	//------------------------------------------------------------------------------------------------------------------------------------------//

	BlinnMaterial::BlinnMaterial() :
		BlinnMaterial("Blinn Material")
	{}
	
	BlinnMaterial::BlinnMaterial(const std::string& name) :
		Material{ name, "Blinn.vert", "Blinn.frag"}
	{
		init();
	}

	void BlinnMaterial::init()
	{
		set_uniform(k_transform_model_to_projection, glm::mat4{ 1.0f });
		set_uniform(k_transform_model_to_world, glm::mat4{ 1.0f });

		for (GLuint i = 0; i < k_num_point_lights; ++i)
		{
			std::string index = std::to_string(i);
			set_uniform(k_point_light + "[" + index + "]." + k_position, glm::vec3{ 0.0f });
			set_uniform(k_point_light + "[" + index + "]." + k_brightness, 0.0f);
			set_uniform(k_point_light + "[" + index + "]." + k_color, glm::vec3{ 1.0f });
			set_sampler_value(k_point_light + "[" + index + "]." + k_depth, 0.0f);
			set_uniform(k_point_light + "[" + index + "]." + k_far_plane, 100.0f);
		}

		for (GLuint i = 0; i < k_num_directional_lights; ++i)
		{
			std::string index = std::to_string(i);
			set_uniform(k_directional_light + "[" + index + "]." + k_direction, glm::vec3{ 0.0f });
			set_uniform(k_directional_light + "[" + index + "]." + k_brightness, 0.0f);
			set_uniform(k_directional_light + "[" + index + "]." + k_color, glm::vec3{ 1.0f });
			set_sampler_value(k_directional_light + "[" + index + "]." + k_depth, 0.0f);
			set_uniform(k_directional_light + "[" + index + "]." + k_projection, glm::mat4{ 1.0f });
		}

		for (GLuint i = 0; i < k_num_spot_lights; ++i)
		{
			std::string index = std::to_string(i);
			set_uniform(k_spot_light + "[" + index + "]." + k_position, glm::vec3{ 0.0f });
			set_uniform(k_spot_light + "[" + index + "]." + k_direction, glm::vec3{ 0.0f });
			set_uniform(k_spot_light + "[" + index + "]." + k_brightness, 0.0f);
			set_uniform(k_spot_light + "[" + index + "]." + k_color, glm::vec3{ 1.0f });
			set_uniform(k_spot_light + "[" + index + "]." + k_inner, 30.0f);
			set_uniform(k_spot_light + "[" + index + "]." + k_outer, 40.0f);
			set_sampler_value(k_spot_light + "[" + index + "]." + k_depth, 0.0f);
			set_uniform(k_spot_light + "[" + index + "]." + k_projection, glm::mat4{ 1.0f });
		}


		set_uniform(k_camera_position, glm::vec3{ 0.0f });

		set_sampler_value(k_material_diffuse, 0.5f);
		set_uniform(k_material_diffuse_amount, 1.0f);
		set_sampler_value(k_material_specular, 0.0f);
		set_uniform(k_material_specular_amount, 0.0f);
		set_sampler_value(k_material_glossiness, 0.5f);
		set_sampler_color(k_material_normal, glm::vec3{ 0.5f, 0.5f, 1.0f });
		set_uniform(k_material_normal_directx_mode, false);
		set_sampler_value(k_material_displacement, 0.0f);
		set_uniform(k_material_displacement_amount, 0.1f);
		set_uniform(k_material_displacement_enabled, false);
	}

	void BlinnMaterial::update_view(CameraNode* camera_node, Node* model_node)
	{
		set_uniform(k_transform_model_to_projection, camera_node->world_to_projection() * model_node->world_to_node());
		set_uniform(k_transform_model_to_world, model_node->world_to_node());
		set_uniform(k_transform_model_world_to_normal, model_node->world_normal_to_node());
		set_uniform(k_camera_position, camera_node->world_position());
	}

	void BlinnMaterial::update_lights(const std::vector<LightNode*>& light_nodes)
	{
		std::string index;
		std::string type = "";

		for (LightNode* light_node : light_nodes)
		{
			PointLight* point_light = dynamic_cast<PointLight*>(light_node->light());
			DirectionalLight* directional_light = dynamic_cast<DirectionalLight*>(light_node->light());
			SpotLight* spot_light = dynamic_cast<SpotLight*>(light_node->light());

			if (point_light)
			{
				update_light(light_node, point_light);
			}
			else if (directional_light)
			{
				update_light(light_node, directional_light);
			}
			else if (spot_light)
			{
				update_light(light_node, spot_light);
			}
		}
	}
	void BlinnMaterial::update_light_transform(LightNode* light_node, CameraNode* camera_node)
	{
		std::string index = std::to_string(light_node->shader_pos());
		std::string type = light_node->light()->type();

		set_uniform(type + "[" + index + "]." + k_projection, camera_node->world_to_projection());
	}


	void BlinnMaterial::update_light(LightNode* light_node, const PointLight* point_light)
	{
		std::string index = std::to_string(light_node->shader_pos());

		set_uniform(k_point_light + "[" + index + "]." + k_position, light_node->world_position());
		set_uniform(k_point_light + "[" + index + "]." + k_brightness, light_node->light()->brightness());
		set_uniform(k_point_light + "[" + index + "]." + k_color, light_node->light()->color());
	}

	void BlinnMaterial::update_light(LightNode* light_node, const DirectionalLight* directional_light)
	{
		std::string index = std::to_string(light_node->shader_pos());

		set_uniform(k_directional_light + "[" + index + "]." + k_direction, light_node->direction());
		set_uniform(k_directional_light + "[" + index + "]." + k_brightness, light_node->light()->brightness());
		set_uniform(k_directional_light + "[" + index + "]." + k_color, light_node->light()->color());
	}

	void BlinnMaterial::update_light(LightNode* light_node, const SpotLight* spot_light)
	{
		std::string index = std::to_string(light_node->shader_pos());

		set_uniform(k_spot_light + "[" + index + "]." + k_position, light_node->world_position());
		set_uniform(k_spot_light + "[" + index + "]." + k_direction, light_node->direction());
		set_uniform(k_spot_light + "[" + index + "]." + k_inner, spot_light->cos_inner_angle());
		set_uniform(k_spot_light + "[" + index + "]." + k_outer, spot_light->cos_outer_angle());
		set_uniform(k_spot_light + "[" + index + "]." + k_brightness, light_node->light()->brightness());
		set_uniform(k_spot_light + "[" + index + "]." + k_color, light_node->light()->color());
	}



	// BLOOM
	//------------------------------------------------------------------------------------------------------------------------------------------//
	BloomMaterial::BloomMaterial() :
		BloomMaterial("Bloom Material")
	{}
	
	BloomMaterial::BloomMaterial(const std::string& name) :
		Material(name, "ScreenPassthrough.vert", "Bloom.frag")
	{
		init();
	}

	void BloomMaterial::init()
	{
		set_sampler_value(k_color, 0.0f);
		set_sampler_value(k_bright, 0.0f);
	}

	// DEPTH
	//------------------------------------------------------------------------------------------------------------------------------------------//
	DepthMaterial::DepthMaterial() :
		DepthMaterial{ "Depth Material" }
	{}
	
	DepthMaterial::DepthMaterial(const std::string& name) :
		Material{ name, "Depth.vert", "Depth.frag" }
	{
		init();
	}

	void DepthMaterial::init()
	{
		set_uniform(k_transform_model_to_projection, glm::mat4{ 1.0f });
	}

	void DepthMaterial::update_view(CameraNode* camera_node, Node* mesh_node)
	{
		set_uniform(k_transform_model_to_projection, camera_node->world_to_projection() * mesh_node->world_to_node());
	}

	// DEPTH CUBE
	//------------------------------------------------------------------------------------------------------------------------------------------//
	DepthCubeMaterial::DepthCubeMaterial() :
		DepthCubeMaterial("Depth Cube Material")
	{}
	
	DepthCubeMaterial::DepthCubeMaterial(const std::string& name) :
		Material(name, "DepthCube.vert", "DepthCube.geom", "DepthCube.frag")
	{
		init();
	}

	void DepthCubeMaterial::init()
	{
		set_uniform(k_transform_model_to_world, glm::mat4{ 1.0f });
		for (GLuint i = 0; i < k_num_faces; ++i)
		{
			set_uniform(k_shadow + "[" + std::to_string(i) + "]." + k_transform, glm::mat4{ 1.0f });
		}
		set_uniform(k_point_light_position, glm::vec3{ 0.0f });
		set_uniform(k_point_light_far_plane, 100.0f);
	}

	void DepthCubeMaterial::update_view(CameraNode* camera_node, Node* model_node)
	{
		set_uniform(k_transform_model_to_world, model_node->world_to_node());
		set_uniform(k_point_light_position, camera_node->world_position());
		set_uniform(k_point_light_far_plane, camera_node->camera()->clip_far());
	}

	// GAUSSIAN BLUR
	//------------------------------------------------------------------------------------------------------------------------------------------//
	GaussianBlurMaterial::GaussianBlurMaterial() :
		GaussianBlurMaterial("Gaussian Blur Material")
	{}

	GaussianBlurMaterial::GaussianBlurMaterial(const std::string& name) :
		Material{name, "ScreenPassthrough.vert", "GaussianBlur.frag"}
	{
		init();
	}

	void GaussianBlurMaterial::init()
	{
		set_sampler_value(k_image, 0.0f);
		set_uniform(k_is_horizontal, true);
	}

	// G-BUFFER
	//------------------------------------------------------------------------------------------------------------------------------------------//
	GBufferMaterial::GBufferMaterial() :
		GBufferMaterial("G-Buffer Material")
	{}

	GBufferMaterial::GBufferMaterial(const std::string& name) :
		Material(name, "GBuffer.vert", "GBuffer.frag")
	{
		init();
	}

	void GBufferMaterial::init()
	{
		set_uniform(k_transform_model_to_projection, glm::mat4{ 1.0f });
		set_uniform(k_transform_model_to_world, glm::mat4{ 1.0f });
		set_uniform(k_transform_model_to_world_normal, glm::mat3{ 1.0f });

		set_sampler_value(k_material_diffuse, 0.0f);
		set_uniform(k_material_diffuse_amount, 1.0f);

		set_sampler_value(k_material_specular, 0.0f);
		set_uniform(k_material_specular_amount, 1.0f);

		set_sampler_value(k_material_glossiness, 0.5f);

		set_sampler_color(k_material_normal, glm::vec3{ 0.5f, 0.5f, 1.0f });
		set_uniform(k_material_normal_directx_mode, false);

		set_sampler_value(k_material_displacement, 0.0f);
		set_uniform(k_material_displacement_amount, 0.0f);
		set_uniform(k_material_displacement_enabled, false);
	}

	void GBufferMaterial::update_view(CameraNode* camera_node, Node* model_node)
	{
		set_uniform(k_transform_model_to_projection,	camera_node->world_to_projection() * model_node->world_to_node());
		set_uniform(k_transform_model_to_world,			model_node->world_to_node());
		set_uniform(k_transform_model_to_world_normal,	model_node->world_normal_to_node());
	}

	// HDR
	//------------------------------------------------------------------------------------------------------------------------------------------//
	HDRMaterial::HDRMaterial() :
		HDRMaterial("HDR Material")
	{}

	HDRMaterial::HDRMaterial(const std::string& name) :
		Material(name, "ScreenPassthrough.vert", "HDR.frag")
	{
		init();
	}

	void HDRMaterial::init()
	{
		set_sampler_value(k_hdr_image, 0.0f);
		set_uniform(k_is_hdr, true);
		set_uniform(k_exposure, 1.0f);
	}


	// LIGHT
	//------------------------------------------------------------------------------------------------------------------------------------------//
	LightMaterial::LightMaterial() :
		LightMaterial("Light Material")
	{}

	LightMaterial::LightMaterial(const std::string& name) :
		Material{name, "Light.vert", "Light.frag"}
	{
		init();
	}

	void LightMaterial::init()
	{
		set_uniform(k_transform_model_to_projection, glm::mat4{ 1.0f });
		set_uniform(k_light_brightness, 0.0f);
		set_uniform(k_light_color, glm::vec3{ 0.0f });
	}


	void LightMaterial::update_view(CameraNode* cameraNode, Node* model_node)
	{
		set_uniform(k_transform_model_to_projection, cameraNode->world_to_projection() * model_node->world_to_node());
	}
} // namespace gl_engine