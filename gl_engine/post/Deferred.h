#ifndef GL_ENGINE_RENDER_DEFERRED_RENDER_H
#define GL_ENGINE_RENDER_DEFERRED_RENDER_H

#include <unordered_set>
#include <unordered_map>

#include <glm/glm.hpp>

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
		void set_color_texture(const std::string& name, Texture* texture);
		void set_depth_texture(Texture* texture);
		void send_color_textures_to_framebuffer();
		void set_color_texture(const std::string& name, Texture texture);
		void set_depth_texture(Texture texture);

		// // ----- MEMBER VARIABLES ----- // //
		GLenum m_target;
		glm::uvec2 m_dimensions;
		Framebuffer* m_g_buffer_FBO = NULL;
		Texture m_g_depth;
		std::unordered_map<std::string, Texture> m_internal_textures;
		std::unordered_map<std::string, Texture*> m_external_textures;
		Material* m_material;
		MeshNode m_mesh_node;
	};

	struct BlinnDeferred : public Deferred
	{
		BlinnDeferred(const GLenum target, Framebuffer* g_buffer, const glm::uvec2& dimensions);

		BlinnDeferredMaterial m_material;
	};

	struct AODeferred : public Deferred
	{
		AODeferred(const GLenum target, Framebuffer* g_buffer, const glm::uvec2& dimensions);

		AO_GBufferMaterial m_material;
	};
}
#endif