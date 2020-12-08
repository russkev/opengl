#ifndef GL_ENGINE_MATERIAL_LIGHT_MATERIAL_H
#define GL_ENGINE_MATERIAL_LIGHT_MATERIAL_H

#include "Material.h"

namespace glen
{
	struct PointLight;
	struct DirectionalLight;
	struct SpotLight;

	// AO
	//------------------------------------------------------------------------------------------------------------------------------------------//
	
	/*!
	 * @brief Helper material for creating screen space ambient occlusion.
	*/
	struct AO_BlurMaterial : public Material
	{
		// @cond
		
		inline static const std::string k_ao_input = "ao_input";
		inline static const std::string k_color_input = "color_input";

		// @endcond

		AO_BlurMaterial();
		AO_BlurMaterial(const std::string& name);
	private:
		void init();
	};

	/*!
	 * @brief Helper material for creating screen space ambient occlusion.
	*/
	struct AO_GBufferMaterial : public Material
	{
		// @cond 

		inline static const std::string k_g_position	= "g_position";
		inline static const std::string k_g_normal		= "g_normal";
		inline static const std::string k_g_diffuse		= "g_diffuse";

		inline static const std::string k_transform_model_to_world		= "transform.model_to_world";
		inline static const std::string k_transform_world_to_cam		= "transform.world_to_cam";
		inline static const std::string k_transform_cam_to_projection	= "transform.cam_to_projection";
		
		// @endcond

		AO_GBufferMaterial();
		AO_GBufferMaterial(const std::string& name);
	private:
		void init();
	public:
		void update_view(const CameraNode* camera_node, const Node* model_node) override;
	};

	/*!
	 * @brief Ambient Occlusion Material.
	 * 
	 * Uses deferred rendering and a post effect to apply ambient occlusion to all objects.
	*/
	struct AO_Material : public Material
	{
		// @cond

		const static constexpr GLuint k_num_samples = 128;

		inline static const std::string k_g_cam_space_position = "g_cam_space_position";
		inline static const std::string k_g_cam_space_normal = "g_cam_space_normal";
		inline static const std::string k_noise = "noise";
		inline static const std::string k_samples = "samples";
		inline static const std::string k_radius = "radius";
		inline static const std::string k_bias = "bias";
		inline static const std::string k_screen_dimensions = "screen_dimensions";
		inline static const std::string k_noise_tile_dimensions = "noise_tile_dimensions";
		inline static const std::string k_cam_to_projection = "cam_to_projection";

		// @endcond

		AO_Material();
		AO_Material(const std::string& name);
	private:
		void init();
	public:
		void update_view(const CameraNode* camera_node, const Node* model_node) override;
	};

	// BLINN
	//------------------------------------------------------------------------------------------------------------------------------------------//
	
	/*!
	 * @brief Primary 'uber' material
	 * 
	 * Designed to be a versatile material that can be used with any mesh
	 * Currently allows for:
	 *   - Diffuse textures
	 *   - Specular textures
	 *   - Glossiness textures (black and white)
	 *   - Normal maps
	 *   - Displacement maps
	*/
	struct BlinnMaterial : public Material
	{
		// @cond

		inline static GLuint k_num_point_lights			= 2;
		inline static GLuint k_num_directional_lights	= 2;
		inline static GLuint k_num_spot_lights			= 5;

		inline static const std::string k_transform_model_to_projection		= "transform.model_to_projection";
		inline static const std::string k_transform_model_to_world			= "transform.model_to_world";
		inline static const std::string k_transform_model_world_to_normal	= "transform.model_to_world_normal";

		inline static const std::string k_point_light		= "pointLight";
		inline static const std::string k_directional_light = "directionalLight";
		inline static const std::string k_spot_light		= "spotLight";

		inline static const std::string k_ambient_light_color = "ambientLight.color";
		inline static const std::string k_ambient_light_brightness = "ambientLight.brightness";

		inline static const std::string k_position		= "position";
		inline static const std::string k_direction		= "direction";
		inline static const std::string k_brightness	= "brightness";
		inline static const std::string k_color			= "color";
		inline static const std::string k_inner			= "inner";
		inline static const std::string k_outer			= "outer";
		inline static const std::string k_depth			= "depth";
		inline static const std::string k_projection	= "projection";
		inline static const std::string k_far_plane		= "far_plane";
		inline static const std::string k_shadow_enabled		= "shadow.enabled";
		inline static const std::string k_shadow_bias			= "shadow.bias";
		inline static const std::string k_shadow_radius			= "shadow.radius";
		inline static const std::string k_shadow_num_samples	= "shadow.num_samples";
		inline static const std::string k_diffuse_enabled		= "diffuse_enabled";
		inline static const std::string k_specular_enabled		= "specular_enabled";

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

		// @endcond

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
	
	/*!
	 * @brief Primary shader for use in deferred mode
	 * 
	 * Currently not as powerful as the `BlinnMaterial` but this can still be useful for scenarios
	 * where lots of lights are required.
	*/
	struct BlinnDeferredMaterial : public Material
	{
		// @cond
		
		const static constexpr GLuint k_num_lights = 3;

		inline static const std::string k_g_position = "g_position";
		inline static const std::string k_g_normal = "g_normal";
		inline static const std::string k_g_diffuse_spec = "g_diffuse_spec";
		//inline static const std::string k_view_position = "view_position";

		inline static const std::string k_lights = "lights";
		inline static const std::string k_position = "position";
		inline static const std::string k_color = "color";

		inline static const std::string k_camera_position = "camera.position";

		// @endcond


		BlinnDeferredMaterial();
		BlinnDeferredMaterial(const std::string& name);
		
		void update_view(const CameraNode* camera_node, const Node* model_node);

		void update_lights(const std::vector<LightNode*>& light_nodes) override;


	private:
		void init();

	};

	// BLOOM
	//------------------------------------------------------------------------------------------------------------------------------------------//
	
	/*!
	 * @brief 2D post effect. Causes very bright parts of the scene to 'glow'. 
	 * This effect mimics real life cameras.
	*/
	struct BloomMaterial : public Material
	{
		// @cond
		
		inline static const std::string k_color		= "color";
		inline static const std::string k_bright	= "bright";
		
		// @endcond

		BloomMaterial();
		BloomMaterial(const std::string& name);

	private:
		void init();

	};

	// CUBE MAP
	//------------------------------------------------------------------------------------------------------------------------------------------//
	/*!
	 * @brief Used for creating textured skyboxes and reflection cubes.
	 * 
	 * Requires six textures in the following order:
	 *   - Positive X (Right)
	 *   - Negative X (Left)
	 *   - Positive Y (Top)
	 *   - Negative Y (Bottom)
	 *   - Positive Z (Back)
	 *   - Negative Z (Front)
	*/
	struct CubeMapMaterial : public Material
	{
		// @cond
		
		inline static const std::string k_transform_model_to_projection = "transform.model_to_projection";
		inline static const std::string k_skybox = "skybox";
		
		// @endcond

		CubeMapMaterial();
		CubeMapMaterial(const std::string& name);

	private:
		void init();
	public:
		void update_view(const CameraNode* camera_node, const Node* model_node);
	};

	// DEPTH
	//------------------------------------------------------------------------------------------------------------------------------------------//	
	
	/*!
	 * @brief Depth material. Various uses. Main use for this engine is for depth mapped shadows.
	*/
	struct DepthMaterial : public Material
	{
		// @cond
		
		inline static const std::string k_transform_model_to_projection = "transform.model_to_projection";
		
		// @endcond

		DepthMaterial();
		DepthMaterial(const std::string& name);

	private:
		void init();
	public:
		void update_view(const CameraNode* camera_node, const Node* model_node) override;
	};

	
	// DEPTH CUBE
	//------------------------------------------------------------------------------------------------------------------------------------------//	
	
	/*!
	 * @brief Depth cube material. Various uses. Main use for this engine is to provide depth mapped shadows for spot lights.
	 * @attention Cube maps are six times as expensive to render as normal depth maps. If given the choice, go for the former.
	*/
	struct DepthCubeMaterial : public Material
	{
		// @cond

		inline static const GLuint k_num_faces = 6;

		inline static const std::string k_transform_model_to_world = "transform.model_to_world";
		inline static const std::string k_shadow = "shadow";
		inline static const std::string k_transform = "transform";
		inline static const std::string k_point_light_position = "pointLight.position";
		inline static const std::string k_point_light_far_plane = "pointLight.far_plane";

		// @endcond

		DepthCubeMaterial();
		DepthCubeMaterial(const std::string& name);

	private:
		void init();
	public:
		void update_view(const CameraNode* camera_node, const Node* model_node);
	
	};

	// GAUSSIAN BLUR
	//------------------------------------------------------------------------------------------------------------------------------------------//
	
	/*!
	 * @brief 2D Post effect. Blurs everything on screen. Useful in `Bloom` effect.
	*/
	struct GaussianBlurMaterial : public Material
	{
		// @cond

		inline static const std::string k_image = "image";
		inline static const std::string k_is_horizontal = "is_horizontal";
		
		// @endcond

		GaussianBlurMaterial();
		GaussianBlurMaterial(const std::string& name);

	private:
		void init();
	public:

	};

	// G-BUFFER
	//------------------------------------------------------------------------------------------------------------------------------------------//	
	
	/*!
	 * @brief Intermediate material for use in deferred rendering situations.
	*/
	struct GBufferMaterial : public Material
	{
		// @cond

		inline static const std::string k_transform = "transform";
		inline static const std::string k_material = "material";

		inline static const std::string k_transform_model_to_projection = k_transform + ".model_to_projection";
		inline static const std::string k_transform_model_to_world = k_transform + ".model_to_world";
		inline static const std::string k_transform_model_to_world_normal = k_transform + ".model_to_world_normal";
		
		inline static const std::string k_material_diffuse = k_material + ".diffuse";
		inline static const std::string k_material_specular = k_material + ".specular";

		// @endcond

		GBufferMaterial();
		GBufferMaterial(const std::string& name);

	private:
		void init();
	public:
		void update_view(const CameraNode* camera_node, const Node* model_node) override;
	};

	// HDR
	//------------------------------------------------------------------------------------------------------------------------------------------//
	
	/*!
	 * @brief 2D Post effect. Allows for tone mapping of values outside the usual 0 to 1 range.
	*/
	struct HDRMaterial : public Material
	{
		// @cond

		inline static const std::string k_hdr_image = "hdr_image";
		inline static const std::string k_is_hdr = "is_hdr";
		inline static const std::string k_exposure = "exposure";

		// @endcond

		HDRMaterial();
		HDRMaterial(const std::string& name);

	private:
		void init();
	};


	// LIGHT
	//------------------------------------------------------------------------------------------------------------------------------------------//
	
	/*!
	 * @brief Simple self illuminated shader. Primarily used for display meshes of lights.
	*/
	struct LightMaterial : public Material
	{
		// @cond

		inline static const std::string k_transform_model_to_projection = "transform.model_to_projection";

		inline static const std::string k_light_brightness	= "light.brightness";
		inline static const std::string k_light_color		= "light.color";

		// @endcond

		LightMaterial();
		LightMaterial(const std::string& name);

		void update_view(const CameraNode* cameraNode, const Node* model_node) override;
	private:
		void init();
	};

}
#endif
