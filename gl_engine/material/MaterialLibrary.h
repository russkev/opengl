#ifndef GL_ENGINE_MATERIAL_LIGHT_MATERIAL_H
#define GL_ENGINE_MATERIAL_LIGHT_MATERIAL_H

#include "Material.h"

#include <tuple>
#include <string>
#include <vector>
#include <map>
#include <set>

#include <glm/glm.hpp>

#include <GL/glew.h>

namespace gl_engine
{
	struct PointLight;
	struct DirectionalLight;
	struct SpotLight;

	struct BlinnMaterial : public Material
	{
		inline static GLuint k_num_point_lights			= 3;
		inline static GLuint k_num_directional_lights	= 3;
		inline static GLuint k_num_spot_lights			= 3;

		inline static const std::string k_transform_model_to_projection		= "transform.model_to_projection";
		inline static const std::string k_transform_model_to_world			= "transform.model_to_world";
		inline static const std::string k_transform_model_world_to_normal	= "transform.model_to_world_normal";

		inline static const std::string k_point_light		= "pointLight";
		inline static const std::string k_directional_light = "directionalLight";
		inline static const std::string k_spot_light		= "spotLight";

		inline static const std::string k_position		= "position";
		inline static const std::string k_direction		= "direction";
		inline static const std::string k_brightness	= "brightness";
		inline static const std::string k_color			= "color";
		inline static const std::string k_inner			= "inner";
		inline static const std::string k_outer			= "outer";
		inline static const std::string k_depth			= "depth";
		inline static const std::string k_projection	= "projection";
		inline static const std::string k_far_plane		= "far_plane";

		inline static const std::string k_camera_position = "camera.position";

		inline static const std::string k_material_diffuse				= "material.diffuse";
		inline static const std::string k_material_diffuse_amount		= "material.diffuse_amount";
		inline static const std::string k_material_specular				= "material.specular";
		inline static const std::string k_material_specular_amount		= "material.specular_amount";
		inline static const std::string k_material_glossiness			= "material.glossiness";
		inline static const std::string k_material_normal				= "material.normal";
		inline static const std::string k_material_normal_directx_mode	= "material.normal_directx_mode";
		inline static const std::string k_material_displacement			= "material.displacement";
		inline static const std::string k_material_displacement_amount	= "material.displacement_amount";
		inline static const std::string k_material_displacement_enabled = "material.displacement_enabled";

		BlinnMaterial(const std::string& name);
	private:
		void init();
	public:
		void update_view(CameraNode* cameraNode, Node* node) override;
		void update_lights(const std::vector<LightNode*>& light_nodes) override;
	private:
		void update_light(LightNode* light_node, const PointLight* point_light);
		void update_light(LightNode* light_node, const DirectionalLight* directional_light);
		void update_light(LightNode* light_node, const SpotLight* spot_light);


	};
	// BLOOM
	//------------------------------------------------------------------------------------------------------------------------------------------//
	struct BloomMaterial : public Material
	{
		inline static const std::string k_color =	"color";
		inline static const std::string k_bright =	"bright";

		BloomMaterial(const std::string& name);

	private:
		void init();

	};


	// LIGHT
	//------------------------------------------------------------------------------------------------------------------------------------------//
	struct LightMaterial : public Material
	{
		inline static const std::string k_transform_model_to_projection = "transform.model_to_projection";

		inline static const std::string k_light_position	= "light.position";
		inline static const std::string k_light_brightness	= "light.brightness";
		inline static const std::string k_light_color		= "light.color";

		LightMaterial(const std::string& name);

		void update_view(CameraNode* cameraNode, Node* node) override;
	private:
		void init();
	};
} // namespace gl_engine


#endif