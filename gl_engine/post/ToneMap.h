#ifndef GL_ENGINE_POST_TONE_MAP_H
#define GL_ENGINE_POST_TONE_MAP_H

#include "PostEffect.h"

#include "../material/Material.h"
#include "../node/MeshNode.h"

namespace gl_engine
{

	/*

		Tonemap HDR image

	*/
	struct ToneMap : public PostEffect
	{
		// // ----- CONSTRUCTOR ----- // //
		ToneMap(Framebuffer* backbuffer, const glm::uvec2* dimensions);

		// // ----- GENERAL METHODS ----- // //
		void draw() override;

		// // ----- GETTERS ----- // //
		Texture* beauty();
		Texture* bright();

		// // ----- MEMBER VARIABLES ----- // //
	private:
		Material m_material{ "HDR Shader", "screenPassthrough.vert", "HDR.frag" };
		MeshNode m_mesh_node{ "HDR Screen Node", PostEffect::mesh(), &m_material };

		Texture m_beauty;
		Texture m_bright;
	};
}

#endif