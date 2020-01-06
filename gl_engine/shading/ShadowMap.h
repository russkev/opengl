#ifndef GL_ENGINE_SHADING_SHADOW_MAP
#define GL_ENGINE_SHADING_SHADOW_MAP

#include <string>

#include <GL/glew.h>

#include "Material.h"
#include "Texture.h"
#include "../camera/Camera.h"
#include "../node/CameraNode.h"

// // ----- FORWARD DECLERATIONS ----- // //
struct Light;
struct LightNode;


namespace gl_engine
{
	/*

		Shadow map struct.
		For dealing with creating and rendering of shadow maps

	*/
	struct ShadowMap
	{
		// // ----- CONSTANTS ----- // //
		static constexpr GLuint SHADOW_WIDTH = 1024;
		static constexpr GLuint SHADOW_HEIGHT = 1024;
		static const std::string MODEL_TRANSFORM;
		static const std::string LIGHT_SPACE_TRANSFORM;
		static const std::string DEPTH_MAP;
		
		static const std::string DEPTH_MAP_NAME;
		static const char* DEPTH_MAP_VERT;
		static const char* DEPTH_MAP_FRAG;
		static const char* CUBE_MAP_VERT;
		static const char* CUBE_MAP_GEOM;
		static const char* CUBE_MAP_FRAG;

		static const std::string SHADOW;
		static const std::string TRANSFORMS;
		static const std::string FAR_PLANE;

		// // ----- CONSTRUCTOR ----- // //
		ShadowMap(LightNode* lightNode);

		// // ----- INITIALIZATION ----- // //
	public:
		void init_materials(std::vector<Material*>& materials);
	private:
		void init_directional_shadowMap();
		void init_point_shadowMap();

		// // ----- UPDATE ----- // //
	public:
		void update_materials(std::vector<Material*>& materials);

		// // ----- RENDER ----- // //
		void render_shadowMap(std::map<std::string, Node*>& root_nodes);
	private:
		void render_directional_shadowMap(std::map<std::string, Node*>& root_nodes);
		void render_point_shadowMap(std::map<std::string, Node*>& root_nodes);

		// // ----- GENERAL METHODS ----- // //
	public:
		bool check_bound_framebuffer();
		bool is_directional();
		bool is_point();

		// // ----- MEMBER VARIABLES ----- // //
	private:
		GLuint		m_depthMap_FBO;
		Material	m_depth_material{ "Depth shader", "DepthShader.vert", "DepthShader.frag" };
		Texture		m_texture;
		LightNode*	m_lightNode = NULL;

		CameraNode	m_cameraNode;
	};
} // namespace gl_engine

#endif