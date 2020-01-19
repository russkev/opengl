#ifndef GL_ENGINE_POST_BLOOM_H
#define GL_ENGINE_POST_BLOOM_H

#include "PostEffect.h"

#include "ToneMap.h"
#include "../shading/Material.h"
#include "../node/MeshNode.h"

namespace gl_engine
{

	/*

		Create glow in bright areas

	*/
	struct Bloom : public PostEffect
	{
		// // ----- CONSTRUCTOR ----- // //
		Bloom(Framebuffer* backbuffer, const glm::uvec2* dimensions, ToneMap* tone_map);

		// // ----- GENERAL METHODS ----- // //
		void draw() override;

		// // ----- MEMBER VARIABLES ----- // //
	private:
		Material m_material{ "Bloom Shader", "screenPassthrough.vert", "Bloom.frag" };
		MeshNode m_mesh_node{ "Bloom Screen Node", PostEffect::mesh(), &m_material };
	};
}

#endif