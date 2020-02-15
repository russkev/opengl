#ifndef GL_ENGINE_POST_BLOOM_H
#define GL_ENGINE_POST_BLOOM_H

#include "PostEffect.h"

#include "shading/Framebuffer.h"
#include "material/Material.h"
#include "material/MaterialLibrary.h"
#include "node/MeshNode.h"

namespace glen
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
		Bloom(Framebuffer* backbuffer, const glm::uvec2& dimensions, ToneMap* tone_map);

		// // ----- GENERAL METHODS ----- // //
		void draw() override;
	private:
		void gaussian_blur();

		// // ----- MEMBER VARIABLES ----- // //
	private:
		// !!! Tonemapping should really be done after the glow effect
		//Framebuffer* m_backbuffer_fbo;
		ToneMap* m_tone_map;

		Framebuffer m_pingpong_fbos[DIRECTIONS];
		Texture m_pingpong_textures[DIRECTIONS];
		Texture m_blur_texture;

		GaussianBlurMaterial m_blur_material{};
		BloomMaterial m_bloom_material{};
		MeshNode m_blur_node{ "Blur Screen Node", PostEffect::mesh(), &m_blur_material };
		MeshNode m_bloom_node{ "Bloom Screen Node", PostEffect::mesh(), &m_bloom_material };
	};
}
#endif