#ifndef GL_ENGINE_SHADING_SHADOW_MAP
#define GL_ENGINE_SHADING_SHADOW_MAP

#include <string>

#include <GL/glew.h>

#include "material/Material.h"
#include "Texture.h"
#include "Framebuffer.h"

#include "camera/Camera.h"
#include "node/CameraNode.h"

// // ----- FORWARD DECLERATIONS ----- // //
struct Light;
struct LightNode;


namespace glen
{
	/*

		Shadow map struct.
		For dealing with creating and rendering of shadow maps

	*/
	struct ShadowMap
	{
		// // ----- CONSTANTS ----- // //
		static const GLuint k_shadow_width;
		static const GLuint k_shadow_height;
		static const GLfloat k_default_clip_near;
		static const GLfloat k_default_clip_far;

		// // ----- CONSTRUCTOR ----- // //
		ShadowMap(LightNode* lightNode);

		// // ----- INITIALIZATION ----- // //
	public:
		void init_materials(std::vector<Material*>& materials);
	private:
		void init_camera();
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
		std::vector<glm::mat4> make_poin_shadow_transforms(const glm::vec3& position);

		// // ----- GENERAL METHODS ----- // //
	public:
		bool check_bound_framebuffer();
		bool is_directional();
		bool is_point();

		// // ----- SETTERS ----- // //
	public:
		void set_clip_near(GLfloat);
		void set_clip_far(GLfloat);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		//Material	m_depth_material;
		std::unique_ptr<Material> m_depth_material;
		Texture		m_texture;
		Framebuffer m_framebuffer;
		LightNode*	m_lightNode = NULL;

		CameraNode	m_cameraNode;
	};
}

#endif