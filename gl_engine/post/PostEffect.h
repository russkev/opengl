#ifndef GL_ENGINE_POST_POST_EFFECT_H
#define GL_ENGINE_POST_POST_EFFECT_H

#include "mesh/Mesh.h"
#include "mesh/WindowQuad.h"

namespace glen
{
	// // -----FORWARD DECLERATION ----- // //
	struct Framebuffer;

	/*
	
		Abstract post effect struct for doing 2D effects
	
	*/
	struct PostEffect
	{
		// // ----- CONSTRUCTOR ----- // //
		PostEffect(Framebuffer* backbuffer)
		{
			m_backbuffer_fbo = backbuffer;
		}

		// // ----- GENERAL METHODS ----- // //
		virtual void draw() {};

		// // ----- GETTERS ----- // //
		Mesh* mesh()
		{
			return &m_mesh;
		}

		Framebuffer* backbuffer_fbo()
		{
			return m_backbuffer_fbo;
		}


		// // ----- MEMBER VARIABLES ----- // //
	private:
		Framebuffer* m_backbuffer_fbo;
		Mesh m_mesh{ WindowQuad::create_windowQuad() };
	};
}
#endif