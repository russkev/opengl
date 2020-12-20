#ifndef GL_ENGINE_SHADING_SHADOW_MAP
#define GL_ENGINE_SHADING_SHADOW_MAP

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
	/*!

		Shadow map struct.
		For dealing with, creating and rendering of shadow maps

	*/
	struct ShadowMap
	{
		// @cond

		//inline static const glm::uvec2 k_shadow_dimensions = glm::uvec2{ 1024, 1024 };
		static constexpr GLfloat k_default_clip_near = 0.1f;
		static constexpr GLfloat k_default_clip_far = 100.0f;
		static constexpr GLfloat k_no_value_float = -1.0f;
		static constexpr GLint k_no_value_int = -1;

		// @endcond

		
		// // ----- CONSTRUCTOR ----- // //
		
		/*!
		 * @brief Constructor
		 * @param lightNode The `LightNode` that should be casting this shadow.
		*/
		ShadowMap(LightNode* lightNode);
		
		/*!
		 * @brief Constructor
		 * @param lightNode The `LightNode` that should be casting this shadow.
		 * @param resolution The width and height dimensions of the shadow map.
		*/
		ShadowMap(LightNode* lightNode, const GLuint resolution);

		/*!
		 * @brief Constructor
		 * @param lightNode The `LightNode` that should be casting this shadow.
		 * @param dimensions `uvec2` with width and height dimensions of the shadow map.
		*/
		ShadowMap(LightNode* lightNode, const glm::uvec2& dimensions);

		// // ----- INITIALIZATION ----- // //
	public:
		/*!
		 * @brief Set up a list of materials to be affected by this shadow map.
		*/
		void init_materials(std::vector<Material*>& materials);
	private:
		void init_camera();
		void init_directional_shadowMap();
		void init_point_shadowMap();

		// // ----- UPDATE ----- // //
	public:
		/*!
		 * @brief Update a list of materials with this shadow map.
		*/
		void update_materials(std::vector<Material*>& materials);

		
		// // ----- RENDER ----- // //
		
		/*!
		 * @brief Render all pre-passes required for making the shadow map.
		 * @param root_nodes All nodes that should be affected by this shadow.
		 * Children of the nodes will be traversed automatically and don't need to be
		 * added seperately.
		*/
		void render_shadowMap(std::map<std::string, Node*>& root_nodes);
	private:
		void render_directional_shadowMap(std::map<std::string, Node*>& root_nodes);
		void render_point_shadowMap(std::map<std::string, Node*>& root_nodes);
		std::vector<glm::mat4> make_poin_shadow_transforms(const glm::vec3& position);

		// // ----- GENERAL METHODS ----- // //
	public:
		/*!
		 * @brief Run diagnostics to check if this framebuffer is valid.
		 * If not, an appropriate message will be printed to the terminal.
		 * @return False if framebuffer not valid.
		*/
		bool check_bound_framebuffer();

		/*!
		 * @return True if the light this shadow is associated with is a directional light.
		*/
		bool is_directional();
		
		/*!
		 * @return True if the light this shadow is associated with is a point light.
		*/
		bool is_point();

		// // ----- GETTERS ----- // //
	public:
		/*!
		 * @return The offset from the mesh from which the shadow is calculated.
		*/
		const GLfloat bias() const;

		/*!
		 * @return The radius of the simulated light. The larger the radius, the softer the shadows.
		 * Keep in mind that a large radius will require more samples to look smooth.
		*/
		const GLfloat radius() const;

		/*!
		 * @brief The square root of the number of samples with which to calculate the shadow.
		 * 
		 * A value of 3 means 9 samples are used.
		*/
		const GLint num_samples() const;

		/*!
		 * @brief The width and height of the depth texture calculated by the shadow map.
		*/
		const GLuint resolution() const;

		// // ----- SETTERS ----- // //
	public:
		/*!
		 * @brief Set the near clipping plane. Items closer than this to the light will not cast shadows.
		*/
		void set_clip_near(const GLfloat);

		/*!
		 * @brief Set the far clipping plane. Items further than this from the light will not cast shadows.
		*/
		void set_clip_far(const GLfloat);

		/*!
		 * @brief Set the offset from the mesh from which the shadow is calculated.
		 * 
		 * If the bias is too low, you'll see artefacts known as 'shadowmap acne'. Too high and the shadows won't appear
		 * connected to the object casting the shadow.
		 * This often requires a bit of tweaking. Often a greater difference between the near and far clipping planes
		 * will require a smaller bias.
		*/
		void set_bias(const GLfloat);

		/*!
		 * @brief Set the radius of the simulated light. The larger the radius, the softer the shadows.
		 * Keep in mind that a large radius will require more samples to look smooth. This can get easily
		 * get very expensive to render.
		*/
		void set_radius(const GLfloat);

		/*!
		 * @brief Set the number of samples used to calculate the shadow.
		 * 
		 * The actual number of samples will be the square of this number so a number of 4 will reault in
		 * 16 pre-passes required to render the shadow map. As you can imagine, too many samples can 
		 * easily slow things down considerably
		*/
		void set_num_samples(const GLint);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		glm::uvec2 m_dimensions;
		std::unique_ptr<Material> m_depth_material;
		Texture		m_texture;
		Framebuffer m_framebuffer;
		LightNode*	m_light_node = NULL;

		CameraNode	m_camera_node;

		GLfloat m_bias = 0.1f;
		GLfloat m_radius = 0.0f;
		GLint   m_num_samples = 1;
	};
}

#endif