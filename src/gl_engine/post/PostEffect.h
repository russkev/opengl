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
		PostEffect()
		{}

		// // ----- GENERAL METHODS ----- // //
		virtual void draw() {};

		// // ----- GETTERS ----- // //
		Mesh* mesh()
		{
			return &m_mesh;
		}


		// // ----- MEMBER VARIABLES ----- // //
	private:
		Mesh m_mesh{ WindowQuad::create_windowQuad() };
	};
}
#endif