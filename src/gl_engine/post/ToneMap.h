#ifndef GL_ENGINE_POST_TONE_MAP_H
#define GL_ENGINE_POST_TONE_MAP_H

#include "PostEffect.h"
#include "material/Material.h"
#include "material/MaterialLibrary.h"
#include "node/MeshNode.h"
#include "shading/Framebuffer.h"

namespace glen
{
	/*!

		Tonemap HDR image.

		Used to render information that would usually be outside the usual 0 to 1 range.

	*/
	struct ToneMap : public PostEffect
	{
		// // ----- CONSTRUCTOR ----- // //

		/*!
		 * @brief Constructor.
		 * @param backbuffer Simple `Framebuffer` object (e.g. `Framebuffer{ GL_FRAMEBUFFER }).
		 * @param dimensions The dimensions of the window.
		*/
		ToneMap(Framebuffer* backbuffer, const glm::uvec2& dimensions);

		// // ----- GENERAL METHODS ----- // //

		/*!
		 * @brief Render the effect to screen.
		*/
		void draw() override;

		// // ----- GETTERS ----- // //
		
		/*!
		 * @return A texture containing the final beauty render.
		*/
		Texture* beauty();

		/*!
		 * @brief A texture containing the un-tonemapped image. 
		*/
		Texture* bright();

		// // ----- MEMBER VARIABLES ----- // //
	private:
		Framebuffer* m_backbuffer_fbo;
		HDRMaterial m_material{};
		MeshNode m_mesh_node{ "HDR Screen Node", PostEffect::mesh(), &m_material };

		Texture m_beauty;
		Texture m_bright;
	};
}
#endif