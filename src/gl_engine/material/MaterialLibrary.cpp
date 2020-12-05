#include "pch.h"
#include "MaterialLibrary.h"

#include "node/Node.h"
#include "node/CameraNode.h"
#include "node/LightNode.h"
#include "light/PointLight.h"
#include "light/DirectionalLight.h"
#include "light/SpotLight.h"

namespace glen
{
	// AO BLUR
	//------------------------------------------------------------------------------------------------------------------------------------------//
	AO_BlurMaterial::AO_BlurMaterial() :
		AO_BlurMaterial("AO Blur Material")
	{}

	AO_BlurMaterial::AO_BlurMaterial(const std::string& name) :
		Material{ name, Material::shaders_dir() + "ScreenPassthrough.vert", Material::shaders_dir() + "AO_Blur.frag" }
	{
		init();
	}

	void AO_BlurMaterial::init()
	{
		set_sampler_value(k_ao_input, 1.0f);
		set_sampler_value(k_color_input, 0.5f);
	}

	// AO G-BUFFER
	//------------------------------------------------------------------------------------------------------------------------------------------//
	AO_GBufferMaterial::AO_GBufferMaterial() :
		AO_GBufferMaterial("AO G-Buffer Material")
	{}

	AO_GBufferMaterial::AO_GBufferMaterial(const std::string& name) :
		Material{ name, Material::shaders_dir() + "AO_GBuffer.vert", Material::shaders_dir() + "AO_GBuffer.frag" }
	{
		init();
	}

	void AO_GBufferMaterial::init()
	{
		glm::mat4 default_transform{ 1.0f };

		set_uniform(k_transform_model_to_world, default_transform);
		set_uniform(k_transform_world_to_cam, default_transform);
		set_uniform(k_transform_cam_to_projection, default_transform);
	}

	void AO_GBufferMaterial::update_view(const CameraNode* camera_node, const Node* model_node)
	{
		set_uniform(k_transform_model_to_world, model_node->world_to_node());
		set_uniform(k_transform_world_to_cam, camera_node->camera()->transform_to_cam(camera_node->world_to_node()));
		set_uniform(k_transform_cam_to_projection, camera_node->camera()->cam_to_projection());
	}

	// AO
	//------------------------------------------------------------------------------------------------------------------------------------------//
	AO_Material::AO_Material() :
		AO_Material("AO Material")
	{}

	AO_Material::AO_Material(const std::string& name) :
		Material{ name, Material::shaders_dir() + "ScreenPassthrough.vert", Material::shaders_dir() + "AO.frag" }
	{
		init();
	}

	void AO_Material::init()
	{
		set_sampler_value(k_g_cam_space_position, 0.0f );
		set_sampler_color(k_g_cam_space_normal, glm::vec3(0.5f, 0.5f, 1.0f));
		set_sampler_value(k_noise, 0.0f);

		std::vector<glm::vec3> samples_vector;
		for (GLuint i = 0; i < k_num_samples; ++i)
		{
			samples_vector.push_back(glm::vec3{ 0.0f });
		};
		set_uniform(k_samples, samples_vector);
		
		set_uniform(k_radius, 1.0f);
		set_uniform(k_bias, 0.05f);

		set_uniform(k_screen_dimensions, glm::uvec2{ 100u });
		set_uniform(k_noise_tile_dimensions, glm::uvec2{ 4u });

		set_uniform(k_cam_to_projection, glm::mat4{ 1.0f });
	}

	void AO_Material::update_view(const CameraNode* camera_node, const Node* model_node)
	{
		//set_uniform(k_camera_position, camera_node->world_position());
		set_uniform(k_cam_to_projection, camera_node->camera()->cam_to_projection());
	}

	// BLINN
	//------------------------------------------------------------------------------------------------------------------------------------------//

	BlinnMaterial::BlinnMaterial() :
		BlinnMaterial("Blinn Material")
	{}
	
	BlinnMaterial::BlinnMaterial(const std::string& name) :
		Material{ name,Material::shaders_dir() + "Blinn.vert",Material::shaders_dir() + "Blinn.frag" }
	{
		init();
	}

	void BlinnMaterial::init()
	{
		assert(Material::program_id());

		glm::vec3 default_normal{ 0.5f, 0.5f, 1.0f };

		set_uniform(k_transform_model_to_projection, glm::mat4{ 1.0f });
		set_uniform(k_transform_model_to_world, glm::mat4{ 1.0f });

		set_uniform(k_ambient_light_color, glm::vec3{ 1.0f });
		set_uniform(k_ambient_light_brightness, 0.0f);

		for (GLuint i = 0; i < k_num_point_lights; ++i)
		{
			std::string index = std::to_string(i);
			set_uniform(k_point_light + "[" + index + "]." + k_position, glm::vec3{ 0.0f });
			set_uniform(k_point_light + "[" + index + "]." + k_brightness, 0.0f);
			set_uniform(k_point_light + "[" + index + "]." + k_color, glm::vec3{ 1.0f });
			set_sampler_value(k_point_light + "[" + index + "]." + k_depth, 0.0f);
			set_uniform(k_point_light + "[" + index + "]." + k_far_plane, 100.0f);
			set_uniform(k_point_light + "[" + index + "]." + k_shadow_enabled, false);
			set_uniform(k_point_light + "[" + index + "]." + k_shadow_bias, 0.0005f);
			set_uniform(k_point_light + "[" + index + "]." + k_shadow_radius, 1.0f);
			set_uniform(k_point_light + "[" + index + "]." + k_shadow_num_samples, 1);
			set_uniform(k_point_light + "[" + index + "]." + k_diffuse_enabled, true);
			set_uniform(k_point_light + "[" + index + "]." + k_specular_enabled, true);
		}

		for (GLuint i = 0; i < k_num_directional_lights; ++i)
		{
			std::string index = std::to_string(i);
			set_uniform(k_directional_light + "[" + index + "]." + k_direction, glm::vec3{ 0.0f });
			set_uniform(k_directional_light + "[" + index + "]." + k_brightness, 0.0f);
			set_uniform(k_directional_light + "[" + index + "]." + k_color, glm::vec3{ 1.0f });
			set_sampler_value(k_directional_light + "[" + index + "]." + k_depth, 0.0f);
			set_uniform(k_directional_light + "[" + index + "]." + k_projection, glm::mat4{ 1.0f });
			set_uniform(k_directional_light + "[" + index + "]." + k_shadow_enabled, false);
			set_uniform(k_directional_light + "[" + index + "]." + k_shadow_bias, 0.0005f);
			set_uniform(k_directional_light + "[" + index + "]." + k_shadow_radius, 1.0f);
			set_uniform(k_directional_light + "[" + index + "]." + k_shadow_num_samples, 1);
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
			set_uniform(k_spot_light + "[" + index + "]." + k_shadow_enabled, false);
			set_uniform(k_spot_light + "[" + index + "]." + k_shadow_bias, 0.0005);
			set_uniform(k_spot_light + "[" + index + "]." + k_shadow_radius, 1.0f);
			set_uniform(k_spot_light + "[" + index + "]." + k_shadow_num_samples, 1);
		}

		set_uniform(k_camera_position, glm::vec3{ 0.0f });

		set_sampler_value(k_material_diffuse, 0.5f);
		set_uniform(k_material_diffuse_amount, 1.0f);
		set_sampler_value(k_material_specular, 0.0f);
		set_uniform(k_material_specular_amount, 0.0f);
		set_sampler_value(k_material_glossiness, 0.5f);
		set_sampler_color(k_material_normal, default_normal);
		set_uniform(k_material_normal_directx_mode, false);
		set_sampler_value(k_material_displacement, 0.0f);
		set_uniform(k_material_displacement_amount, 0.1f);
		set_uniform(k_material_displacement_enabled, false);
	}

	void BlinnMaterial::update_view(const CameraNode* camera_node, const Node* model_node)
	{
		set_uniform(k_transform_model_to_projection, camera_node->world_to_projection() * model_node->world_to_node());
		set_uniform(k_transform_model_to_world, model_node->world_to_node());
		//set_uniform(k_transform_model_world_to_normal, model_node->world_normal_to_node());
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

	void BlinnMaterial::update_light_transform(const LightNode* light_node, const CameraNode* camera_node)
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
		set_uniform(k_point_light + "[" + index + "]." + k_diffuse_enabled, light_node->light()->diffuse_enabled());
		set_uniform(k_point_light + "[" + index + "]." + k_specular_enabled, light_node->light()->specular_enabled());
	}

	void BlinnMaterial::update_light(LightNode* light_node, const DirectionalLight* directional_light)
	{
		std::string index = std::to_string(light_node->shader_pos());

		set_uniform(k_directional_light + "[" + index + "]." + k_direction, light_node->direction());
		set_uniform(k_directional_light + "[" + index + "]." + k_brightness, light_node->light()->brightness());
		set_uniform(k_directional_light + "[" + index + "]." + k_color, light_node->light()->color());
		set_uniform(k_directional_light + "[" + index + "]." + k_diffuse_enabled, light_node->light()->diffuse_enabled());
		set_uniform(k_directional_light + "[" + index + "]." + k_specular_enabled, light_node->light()->specular_enabled());
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
		set_uniform(k_spot_light + "[" + index + "]." + k_diffuse_enabled, light_node->light()->diffuse_enabled());
		set_uniform(k_spot_light + "[" + index + "]." + k_specular_enabled, light_node->light()->specular_enabled());
	}

	// BLINN DEFERRED
	//------------------------------------------------------------------------------------------------------------------------------------------//
	BlinnDeferredMaterial::BlinnDeferredMaterial() :
		BlinnDeferredMaterial("Blinn Deferred Material")
	{}

	BlinnDeferredMaterial::BlinnDeferredMaterial(const std::string& name) :
		Material(name, Material::shaders_dir() + "ScreenPassthrough.vert", Material::shaders_dir() + "BlinnDeferred.frag")
	{
		init();
	}

	void BlinnDeferredMaterial::init()
	{
		set_sampler_value(k_g_position, 0.0f);
		set_sampler_value(k_g_normal, 0.0f);
		set_sampler_value(k_g_diffuse_spec, 0.0f);
		//set_uniform(k_view_position, glm::vec3{ 0.0f });

		for (GLuint i = 0; i < k_num_lights; ++i)
		{
			set_uniform(k_lights + "[" + std::to_string(i) + "]." + k_position,	glm::vec3(0.0f));
			set_uniform(k_lights + "[" + std::to_string(i) + "]." + k_color,	glm::vec3(0.0f));
		}

		set_uniform(k_camera_position, glm::vec3(0.0f));
	}

	void BlinnDeferredMaterial::update_view(const CameraNode* camera_node, const Node* model_node)
	{
		set_uniform(k_camera_position, camera_node->world_position());
	}

	void BlinnDeferredMaterial::update_lights(const std::vector<LightNode*>& light_nodes)
	{
		for (int i = 0; i < light_nodes.size(); ++i)
		{
			std::string index = std::to_string(i);
			set_uniform(k_lights + "[" + index + "]." + k_position, light_nodes.at(i)->world_position());
			set_uniform(k_lights + "[" + index + "]." + k_color, light_nodes.at(i)->light()->color());
		}
	
	}

	// BLOOM
	//------------------------------------------------------------------------------------------------------------------------------------------//
	BloomMaterial::BloomMaterial() :
		BloomMaterial("Bloom Material")
	{}
	
	BloomMaterial::BloomMaterial(const std::string& name) :
		Material(name, Material::shaders_dir() + "ScreenPassthrough.vert", Material::shaders_dir() + "Bloom.frag")
	{
		init();
	}

	void BloomMaterial::init()
	{
		set_sampler_value(k_color, 0.0f);
		set_sampler_value(k_bright, 0.0f);
	}

	// COMPOSITE
	//------------------------------------------------------------------------------------------------------------------------------------------//
	CompositeMaterial::CompositeMaterial() :
		CompositeMaterial("Composite Material")
	{}

	CompositeMaterial::CompositeMaterial(const std::string& name) :
		Material(name, Material::shaders_dir() + "ScreenPassthrough.vert", Material::shaders_dir() + "Composite.frag")
	{
		init();
	}

	void CompositeMaterial::init()
	{
		set_sampler_value(k_base, 0.0f);
		set_sampler_value(k_layer_1, 0.0f);
	}

	// CUBE MAP
	//------------------------------------------------------------------------------------------------------------------------------------------//
	CubeMapMaterial::CubeMapMaterial() :
		CubeMapMaterial("Cube Map Material")
	{}

	CubeMapMaterial::CubeMapMaterial(const std::string& name) :
		Material(name, Material::shaders_dir() + "Skybox.vert", Material::shaders_dir() + "Skybox.frag")
	{
		init();
	}

	void CubeMapMaterial::init()
	{
		//set_uniform("skybox", 0);
	}

	void CubeMapMaterial::update_view(const CameraNode* camera_node, const Node* model_node)
	{
		auto view = camera_node->world_to_cam();
		auto projection = camera_node->camera()->cam_to_projection();
		view = glm::mat4(glm::mat3(view));
		//p_matrix = glm::mat4(glm::mat3(p_matrix));
		set_uniform(k_transform_model_to_projection, projection * view);	
	}

	// DEPTH
	//------------------------------------------------------------------------------------------------------------------------------------------//
	DepthMaterial::DepthMaterial() :
		DepthMaterial{ "Depth Material" }
	{}
	
	DepthMaterial::DepthMaterial(const std::string& name) :
		Material{ name, Material::shaders_dir() + "Depth.vert", Material::shaders_dir() + "Depth.frag" }
	{
		init();
	}

	void DepthMaterial::init()
	{
		//
	}

	void DepthMaterial::update_view(const CameraNode* camera_node, const Node* mesh_node)
	{
		set_uniform(k_transform_model_to_projection, camera_node->world_to_projection() * mesh_node->world_to_node());
	}


	// DEPTH CUBE
	//------------------------------------------------------------------------------------------------------------------------------------------//
	DepthCubeMaterial::DepthCubeMaterial() :
		DepthCubeMaterial("Depth Cube Material")
	{}
	
	DepthCubeMaterial::DepthCubeMaterial(const std::string& name) :
		Material(name, Material::shaders_dir() + "DepthCube.vert", Material::shaders_dir() + "DepthCube.geom", Material::shaders_dir() + "DepthCube.frag")
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

	void DepthCubeMaterial::update_view(const CameraNode* camera_node, const Node* model_node)
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
		Material{name, Material::shaders_dir() + "ScreenPassthrough.vert", Material::shaders_dir() + "GaussianBlur.frag" }
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
		Material(name, Material::shaders_dir() + "GBuffer.vert", Material::shaders_dir() + "GBuffer.frag")
	{
		init();
	}

	void GBufferMaterial::init()
	{
		glm::vec3 default_normal{ 0.5f, 0.5f, 1.0f };

		set_uniform(k_transform_model_to_projection, glm::mat4{ 1.0f });
		set_uniform(k_transform_model_to_world, glm::mat4{ 1.0f });
		set_uniform(k_transform_model_to_world_normal, glm::mat3{ 1.0f });

		set_sampler_value(k_material_diffuse, 0.0f);
		set_sampler_value(k_material_specular, 0.0f);
	}

	void GBufferMaterial::update_view(const CameraNode* camera_node, const Node* model_node)
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
		Material(name, Material::shaders_dir() + "ScreenPassthrough.vert", Material::shaders_dir() + "HDR.frag")
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
		Material{name, Material::shaders_dir() + "Light.vert", Material::shaders_dir() + "Light.frag" }
	{
		init();
	}

	void LightMaterial::init()
	{
		set_uniform(k_transform_model_to_projection, glm::mat4{ 1.0f });
		set_uniform(k_light_brightness, 0.0f);
		set_uniform(k_light_color, glm::vec3{ 0.0f });
	}


	void LightMaterial::update_view(const CameraNode* cameraNode, const Node* model_node)
	{
		set_uniform(k_transform_model_to_projection, cameraNode->world_to_projection() * model_node->world_to_node());
	}
}