#ifndef GL_ENGINE_SHADING_SHADOW_MAP
#define GL_ENGINE_SHADING_SHADOW_MAP

#include <GL/glew.h>

#include "Material.h"
#include "Texture.h"
#include "../camera/OrthoCamera.h"
#include "../node/CameraNode.h"

// // ----- FORWARD DECLERATION ----- // //
struct Light;


namespace gl_engine
{
	/*

		Shadow map struct.
		For dealing with creating and rendering of shadow maps

	*/
	struct ShadowMap
	{
		static constexpr GLuint SHADOW_WIDTH = 1024;
		static constexpr GLuint SHADOW_HEIGHT = 1024;

		// // ----- MEMBER VARIABLES ----- // //
		GLuint		m_depthMap_FBO;
		Material	m_depthMaterial;
		Texture		m_texture{ GL_TEXTURE_2D };

		OrthoCamera m_orthoCam{};
		CameraNode m_orthoCam_node{ "ortho cam for light", &m_orthoCam };

		GLuint m_depthMap_ID;


		// // ----- CONSTRUCTOR ----- // //
		ShadowMap();

		// // ----- GENERAL ----- // //
		void init_shadowMap();
		void init_materials(std::vector<Material*>& materials);
		void update_materials(std::vector<Material*>& materials);
		void render_shadowMap(std::map<std::string, Node*>& root_nodes);

		// // ----- GETTERS ----- // //
		
		// // ----- SETTERS ----- // //
		void setLightNode(Node* light_node);


	};
} // namespace gl_engine

#endif