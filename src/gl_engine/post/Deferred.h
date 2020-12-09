#ifndef GL_ENGINE_RENDER_DEFERRED_RENDER_H
#define GL_ENGINE_RENDER_DEFERRED_RENDER_H

#include "PostEffect.h"
#include "shading/Framebuffer.h"
#include "shading/Texture.h"
#include "mesh/Mesh.h"
#include "mesh/WindowQuad.h"
#include "material/MaterialLibrary.h"
#include "node/MeshNode.h"

namespace glen
{
	// // ----- FORWARD DECLERATION ----- // //
	struct CameraNode;

	/*!
	
	Struct for adding a deferred render stage to the primary renderer.

	The deferred stage allows images to be rendered based on pre-passes containing data 
	(e.g. normals, position etc.) intead of image output. This way the final render stage
	just needs to make calculations based off pixel data instead of 3D geometry.

	*/
	struct Deferred : public PostEffect
	{
		// @cond

		// // ----- TYPEDEEFS ----- // //
		
		typedef std::vector <Texture*>		texture_references_t;
		typedef std::map < GLuint, Texture> textures_t;

		// @endcond

		// // ----- CONSTRUCTOR ----- // //

		/*!
		 * @brief Constructor
		 * @param target The GL target to use (e.g. `GL_TEXTURE_2D`).
		 * @param g_buffer The `Framebuffer` object to use.
		 * @param material The `Material` to use. Should be specific G-Buffer material (`AO_GBufferMaterial` for example).
		 * @param dimensions The dimensions of the window.
		*/
		Deferred(const GLenum target, Framebuffer* g_buffer, Material* material, const glm::uvec2& dimensions);

		/*!
		 * @brief Use move constructor instead of copy constructor.
		*/
		Deferred(const Deferred& other) = delete;

		/*!
		 * @brief Move constructor.
		*/
		Deferred(Deferred&& other) noexcept;

		/*!
		 * @brief Use move assign instead of copy assign.
		*/
		Deferred& operator = (const Deferred& other) = delete;

		/*!
		 * @brief Move assign.
		*/
		Deferred& operator = (Deferred&& other) noexcept;
		~Deferred() = default;

		// // ----- GENERAL ----- // //
	private:
		void relink_internal_framebuffer_color_textures(const std::vector<const Texture*>& framebuffer_textures);
	public:
		/*!
		 * @brief Ensure the `Framebuffer` associated with this object is 'active' in relation to OpenGL.
		*/
		void bind();

		/*!
		 * @brief Unbind the `Framebuffer`.
		*/
		void unbind();

		/*!
		 * @brief Update the `Material` associated with this object with the provided `camera_node` information.
		*/
		void update_view(const CameraNode* camera_node);

		/*!
		 * @brief Render to the screen.
		*/
		void draw() override;

		// // ----- GETTERS ----- // //

		/*!
		 * @return Internal dimensions.
		*/
		glm::uvec2 dimensions();

		/*!
		 * @return The framebuffer object.
		*/
		Framebuffer* framebuffer();

		/*!
		 * @return The G-Buffer material.
		*/
		Material* material();

		/*!
		 * @return The rectangle screen mesh.
		*/
		MeshNode* mesh_node();

		/*!
		 * @return The internal g-buffer texture at index `g_buffer_loc`
		*/
		Texture* texture(const GLuint g_buffer_loc);

		/*!
		 * @return The internal depth texture
		*/
		const Texture* depth_texture();


		// // ----- SETTERS ----- // //

		/*!
		 * @brief Set the GL target (e.g. `GL_TEXTURE_2D`).
		*/
		void set_target(const GLenum target);

		/*!
		 * @brief Set the screen dimensions to be used.
		*/
		void set_dimensions(const glm::uvec2& dimensions);

		/*!
		 * @brief Set a color g-buffer texture at index `g_buffer_loc`
		*/
		void set_color_texture(const GLuint g_buffer_loc, Texture* texture);

		/*!
		 * @brief Set a color g-buffer texture at index `g_buffer_loc` and also store it in
		 * this object.
		*/
		void set_color_texture(const GLuint g_buffer_loc, Texture texture);

		/*!
		 * @brief Set the internal depth texture.
		*/
		void set_depth_texture(Texture* texture);

		/*!
		 * @brief Set the internal depth texture and also store it in this object.
		*/
		void set_depth_texture(Texture texture);

		/*!
		 * @brief Pass all color textures to the associated `Framebuffer`
		*/
		void send_color_textures_to_framebuffer();


		// // ----- MEMBER VARIABLES ----- // //
	protected:
		glm::uvec2 m_dimensions;
		
	private:
		GLenum m_target;
		Framebuffer* m_g_buffer_FBO = NULL;
		Texture m_g_depth;
		Texture m_null_texture;
		std::map <GLuint, Texture*> m_all_textures;
		std::map < GLuint, Texture> m_internal_textures;

		Material* m_material;
		MeshNode m_mesh_node;
	};

	/*!
	 * @brief Used to render a blinn shader in deferred mode.
	*/
	struct BlinnDeferred : public Deferred
	{
		/*!
		 * @brief Constructor.
		 * @param target GL target (e.g. `GL_TEXTURE_2D`).
		 * @param g_buffer Simple `Framebuffer` object (e.g. `Framebuffer{ GL_FRAMEBUFFER }`).
		 * @param dimensions The dimensions of the window.
		*/
		BlinnDeferred(const GLenum target, Framebuffer* g_buffer, const glm::uvec2& dimensions);

	private:
		BlinnDeferredMaterial m_material;
	};

	/*!
	 * @brief Used to render ambient occlusion
	*/
	struct AO_GBufferDeferred : public Deferred
	{
		/*!
		 * @brief Constructor
		 * @param target GL target (e.g. `GL_TEXTURE_2D`)
		 * @param g_buffer Simple `Frmabuffer` object (e.g. `Framebuffer{ GL_FRAMEBUFFER }`)
		 * @param dimensions The dimensions of the window.
		*/
		AO_GBufferDeferred(const GLenum target, Framebuffer* g_buffer, const glm::uvec2& dimensions);

	private:
		void init_kernal();
		float increase_nearby_samples(const GLuint i, const GLuint num_samples);
		void init_noise();

		std::uniform_real_distribution<GLfloat> m_random_floats{ 0.0f, 1.0f };
		std::default_random_engine m_generator;

		AO_Material m_ao_material;

		glm::uvec2 m_noise_tile_dimensions{ 4, 4 };
		std::vector<glm::vec3> m_kernal, m_noise_tile;
		Texture m_noise_tile_texture{ GL_TEXTURE_2D };
		Texture m_ao_texture{ Texture::create_bw_null_texture(GL_TEXTURE_2D, m_dimensions) };
	};

	/*!
	 * @brief Used to render the second step in screen space ambient occlusion. 
	 * The initial stage produces an image which is very grainy. This stage blurs
	 * the graininess to make the result look smoother on screen.
	*/
	struct AO_BlurDeferred : public Deferred
	{
		/*!
		 * @brief Constructor
		 * @param target GL target (e.g. `GL_TEXTURE_2D`)
		 * @param g_buffer Simple `Frmabuffer` object (e.g. `Framebuffer{ GL_FRAMEBUFFER }`)
		 * @param dimensions The dimensions of the window.
		*/
		AO_BlurDeferred(const GLenum target, Framebuffer* g_buffer, const glm::uvec2& dimensions);

	private:
		AO_BlurMaterial m_material;
	};
}
#endif