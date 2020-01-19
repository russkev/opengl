#ifndef GL_ENGINE_POST_BLOOM_H
#define GL_ENGINE_POST_BLOOM_H

#include "PostEffect.h"

#include "../shading/Framebuffer.h"
#include "../shading/Material.h"
#include "../node/MeshNode.h"

namespace gl_engine
{
	// // ----- FORWARD DECLERATION ----- // //
	struct ToneMap;

	/*

		Create glow in bright areas

	*/
	struct Bloom : public PostEffect
	{
		// // ----- CONSTANTS ----- // //
		static constexpr GLuint DIRECTIONS = 2;
		static constexpr GLuint PASSES = 10;

		// // ----- CONSTRUCTOR ----- // //
		Bloom(Framebuffer* backbuffer, const glm::uvec2* dimensions, ToneMap* tone_map);

		// // ----- GENERAL METHODS ----- // //
		void draw() override;
	private:
		void gaussian_blur();

		// // ----- MEMBER VARIABLES ----- // //
	private:
		ToneMap* m_tone_map;

		Framebuffer m_pingpong_fbos[DIRECTIONS];
		Texture m_pingpong_textures[DIRECTIONS];
		Texture m_temp_texture;

		Material m_material{ "Gaussian Blur Shader", "ScreenPassthrough.vert", "GaussianBlur.frag" };
		MeshNode m_mesh_node{ "Bloom Screen Node", PostEffect::mesh(), &m_material };
	};
}

#endif