#ifndef GL_ENGINE_RENDER_DEFERRED_RENDER_H
#define GL_ENGINE_RENDER_DEFERRED_RENDER_H

#include <unordered_set>
#include <map>
#include <random>

#include <glm/glm.hpp>
#include <GL/glew.h>

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

	/*
	
	Struct for adding a deferred render stage to the primary renderer

	*/
	struct Deferred : public PostEffect
	{
		// // ----- TYPEDEEFS ----- // //
		typedef std::vector <Texture*>		texture_references_t;
		typedef std::map < GLuint, Texture> textures_t;

		// // ----- CONSTRUCTOR ----- // //
		Deferred() {};
		Deferred(const GLenum target, Framebuffer* g_buffer, Material* material, const glm::uvec2& dimensions);
		Deferred(const Deferred& other) = delete;
		Deferred(Deferred&& other);
		Deferred& operator = (const Deferred& other) = delete;
		Deferred& operator = (Deferred&& other);
		~Deferred() = default;

		// // ----- GENERAL ----- // //
	private:
		void relink_framebuffer_color_textures(const std::vector<const Texture*>& framebuffer_textures);
	public:
		void bind();
		void unbind();
		void update_view(const CameraNode* camera_node);
		void draw() override;

		// // ----- GETTERS ----- // //
		glm::uvec2 dimensions();
		Framebuffer* framebuffer();
		Material* material();
		MeshNode* mesh_node();
		const Texture* texture(const std::string& name);
		const Texture* depth_texture();


		// // ----- SETTERS ----- // //
		void set_target(const GLenum target);
		void set_dimensions(const glm::uvec2& dimensions);
		void set_color_texture(const GLuint g_buffer_location, Texture* texture);
		void set_color_texture(const GLuint g_buffer_location, Texture texture);
		void set_depth_texture(Texture* texture);
		void set_depth_texture(Texture texture);
		void send_color_textures_to_framebuffer();


		// // ----- MEMBER VARIABLES ----- // //
		GLenum m_target;
		glm::uvec2 m_dimensions;
		Framebuffer* m_g_buffer_FBO = NULL;
		Texture m_g_depth;
		Texture m_null_texture{ Texture::create_bw_null_texture(GL_TEXTURE_2D, {1u, 1u}) };
		//std::unordered_map<std::string, Texture> m_internal_textures;
		//std::unordered_map<std::string, Texture*> m_external_textures;
		std::map <GLuint, Texture*> m_all_textures;
		std::map < GLuint, Texture> m_internal_textures;

		Material* m_material;
		MeshNode m_mesh_node;
	};

	struct BlinnDeferred : public Deferred
	{
		BlinnDeferred(const GLenum target, Framebuffer* g_buffer, const glm::uvec2& dimensions);

		BlinnDeferredMaterial m_material;
	};

	struct AO_GBufferDeferred : public Deferred
	{
		AO_GBufferDeferred(const GLenum target, Framebuffer* g_buffer, const glm::uvec2& dimensions);

		//void init_kernal();
		//float increase_nearby_samples(const unsigned int i, const unsigned int num_samples);
		//void init_noise();

		//std::uniform_real_distribution<GLfloat> m_random_floats{ 0.0f, 1.0f };
		//std::default_random_engine m_generator;

		AO_Material m_material;
		//BlinnDeferredMaterial m_material;


		//glm::uvec2 m_noise_tile_dimensions{ 4, 4 };
		//std::vector<glm::vec3> m_kernal, m_noise_tile;
		//Texture m_noise_tile_texture;
		//Texture m_ao_texture{ Texture::create_bw_null_texture(GL_TEXTURE_2D, m_dimensions) };
	};

	//struct AO_Deferred : public Deferred
	//{
	//	AO_Deferred(const GLenum target, Framebuffer* g_buffer, const glm::uvec2& dimensions);

	//	//AO_Material m_material;
	//	AO_GBufferMaterial m_material;

	//};
}
#endif