#ifndef GL_ENGINE_SHADING_SHADOW_MAP
#define GL_ENGINE_SHADING_SHADOW_MAP

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

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
		//inline static const glm::uvec2 k_shadow_dimensions = glm::uvec2{ 1024, 1024 };
		static constexpr GLfloat k_default_clip_near = 0.1f;
		static constexpr GLfloat k_default_clip_far = 1000.0f;
		static constexpr GLfloat k_no_value_float = -1.0f;
		static constexpr GLint k_no_value_int = -1;

		// // ----- CONSTRUCTOR ----- // //
		ShadowMap(LightNode* lightNode);
		ShadowMap(LightNode* lightNode, const GLuint resolution);
		ShadowMap(LightNode* lightNode, const glm::uvec2& dimensions);

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

		// // ----- GETTERS ----- // //
	public:
		const GLfloat bias() const;
		const GLfloat radius() const;
		const GLint num_samples() const;
		const GLuint resolution() const;

		// // ----- SETTERS ----- // //
	public:
		void set_clip_near(const GLfloat);
		void set_clip_far(const GLfloat);
		void set_bias(const GLfloat);
		void set_radius(const GLfloat);
		void set_num_samples(const GLint);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		glm::uvec2 m_dimensions;
		std::unique_ptr<Material> m_depth_material;
		Texture		m_texture;
		Framebuffer m_framebuffer;
		LightNode*	m_light_node = NULL;

		CameraNode	m_camera_node;

		GLfloat m_bias = k_no_value_float;
		GLfloat m_radius = k_no_value_float;
		GLint m_num_samples = k_no_value_int;
	};
}

#endif