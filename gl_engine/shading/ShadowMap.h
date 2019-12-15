#ifndef GL_ENGINE_SHADING_SHADOW_MAP
#define GL_ENGINE_SHADING_SHADOW_MAP

#include <GL/glew.h>

#include "Shader.h"

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
		GLuint m_depthMap_FBO;
		GLuint m_depthMap_ID;
		Shader m_depthShader;

		// // ----- GENERAL ----- // //
		void init_shadowMap();
		void render_shadowMap(const glm::mat4 & model_matrix);
	};
} // namespace gl_engine

#endif