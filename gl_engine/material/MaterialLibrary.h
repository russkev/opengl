#ifndef GL_ENGINE_MATERIAL_LIGHT_MATERIAL_H
#define GL_ENGINE_MATERIAL_LIGHT_MATERIAL_H

#include <tuple>
#include <string>
#include <vector>
#include <map>
#include <set>

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Material.h"

namespace glen
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

		BlinnMaterial();
		BlinnMaterial(const std::string& name);

	private:
		void init();
	public:
		void update_view(const CameraNode* camera_node, const Node* model_node) override;
		void update_lights(const std::vector<LightNode*>& light_nodes) override;
		void update_light_transform(const LightNode* light_node, const CameraNode* camera_node) override;

	private:
		void update_light(LightNode* light_node, const PointLight* point_light);
		void update_light(LightNode* light_node, const DirectionalLight* directional_light);
		void update_light(LightNode* light_node, const SpotLight* spot_light);
	};

	// BLINN DEFERRED
	//------------------------------------------------------------------------------------------------------------------------------------------//
	struct BlinnDeferredMaterial : public Material
	{
		const static constexpr GLuint k_num_lights = 3;

		inline static const std::string k_g_position = "g_position";
		inline static const std::string k_g_normal = "g_normal";
		inline static const std::string k_g_diffuse_spec = "g_diffuse_spec";
		//inline static const std::string k_view_position = "view_position";

		inline static const std::string k_lights = "lights";
		inline static const std::string k_position = "position";
		inline static const std::string k_color = "color";

		inline static const std::string k_camera_position = "camera.position";

		BlinnDeferredMaterial();
		BlinnDeferredMaterial(const std::string& name);
		
		void update_view(const CameraNode* camera_node, const Node* model_node);

		void update_lights(const std::vector<LightNode*>& light_nodes) override;


	private:
		void init();

	};

	// BLOOM
	//------------------------------------------------------------------------------------------------------------------------------------------//
	struct BloomMaterial : public Material
	{
		inline static const std::string k_color		= "color";
		inline static const std::string k_bright	= "bright";

		BloomMaterial();
		BloomMaterial(const std::string& name);

	private:
		void init();

	};

	// DEPTH
	//------------------------------------------------------------------------------------------------------------------------------------------//	
	struct DepthMaterial : public Material
	{
		inline static const std::string k_transform_model_to_projection = "transform.model_to_projection";

		DepthMaterial();
		DepthMaterial(const std::string& name);

	private:
		void init();
	public:
		void update_view(const CameraNode* camera_node, const Node* model_node) override;
	};

	// DEPTH CUBE
	//------------------------------------------------------------------------------------------------------------------------------------------//	
	struct DepthCubeMaterial : public Material
	{
		inline static const GLuint k_num_faces = 6;

		inline static const std::string k_transform_model_to_world = "transform.model_to_world";
		inline static const std::string k_shadow = "shadow";
		inline static const std::string k_transform = "transform";
		inline static const std::string k_point_light_position = "pointLight.position";
		inline static const std::string k_point_light_far_plane = "pointLight.far_plane";
	
		DepthCubeMaterial();
		DepthCubeMaterial(const std::string& name);

	private:
		void init();
	public:
		void update_view(const CameraNode* camera_node, const Node* model_node);
	
	};

	// GAUSSIAN BLUR
	//------------------------------------------------------------------------------------------------------------------------------------------//
	struct GaussianBlurMaterial : public Material
	{
		inline static const std::string k_image = "image";
		inline static const std::string k_is_horizontal = "is_horizontal";

		GaussianBlurMaterial();
		GaussianBlurMaterial(const std::string& name);

	private:
		void init();
	public:

	};

	// G-BUFFER
	//------------------------------------------------------------------------------------------------------------------------------------------//	
	struct GBufferMaterial : public Material
	{
		inline static const std::string k_transform = "transform";
		inline static const std::string k_material = "material";

		inline static const std::string k_transform_model_to_projection = k_transform + ".model_to_projection";
		inline static const std::string k_transform_model_to_world = k_transform + ".model_to_world";
		inline static const std::string k_transform_model_to_world_normal = k_transform + ".model_to_world_normal";
		
		inline static const std::string k_material_diffuse = k_material + ".diffuse";
		inline static const std::string k_material_diffuse_amount = k_material + ".diffuse_amount";

		inline static const std::string k_material_specular = k_material + ".specular";
		inline static const std::string k_material_specular_amount = k_material + ".specular_amount";
		
		inline static const std::string k_material_glossiness = k_material + ".glossiness";
		
		inline static const std::string k_material_normal = k_material + ".normal";
		inline static const std::string k_material_normal_directx_mode = k_material + ".normal_directx_mode";

		inline static const std::string k_material_displacement = k_material + ".displacement";
		inline static const std::string k_material_displacement_amount = k_material + ".diaplcement_amount";
		inline static const std::string k_material_displacement_enabled = k_material + ".displacement_enabled";

		GBufferMaterial();
		GBufferMaterial(const std::string& name);

	private:
		void init();
	public:
		void update_view(const CameraNode* camera_node, const Node* model_node) override;
	};

	// HDR
	//------------------------------------------------------------------------------------------------------------------------------------------//
	struct HDRMaterial : public Material
	{
		inline static const std::string k_hdr_image = "hdr_image";
		inline static const std::string k_is_hdr = "is_hdr";
		inline static const std::string k_exposure = "exposure";

		HDRMaterial();
		HDRMaterial(const std::string& name);

	private:
		void init();
	};

	// LIGHT
	//------------------------------------------------------------------------------------------------------------------------------------------//
	struct LightMaterial : public Material
	{
		inline static const std::string k_transform_model_to_projection = "transform.model_to_projection";

		inline static const std::string k_light_brightness	= "light.brightness";
		inline static const std::string k_light_color		= "light.color";

		LightMaterial();
		LightMaterial(const std::string& name);

		void update_view(const CameraNode* cameraNode, const Node* model_node) override;
	private:
		void init();
	};
}
#endif
