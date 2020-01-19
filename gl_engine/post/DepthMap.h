#ifndef GL_ENGINE_POST_DEPTH_MAP_H
#define GL_ENGINE_POST_DEPTH_MAP_H

#include "PostEffect.h"

#include "../shading/Texture.h"

namespace gl_engine
{

	/*

		Basic depth map helper struct

	*/
	struct DepthMap : public PostEffect
	{
		// // ----- CONSTRUCTOR ----- // //
		DepthMap(Framebuffer* backbuffer, const glm::uvec2* dimensions);

		// // ----- GENERAL METHODS ----- // //
		void draw() override {};

		// // ----- MEMBER VARIABLES ----- // //
	private:
		Texture m_depth;
	};
}

#endif