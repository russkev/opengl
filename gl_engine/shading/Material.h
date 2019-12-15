#ifndef GL_ENGINE_SHADING_MATERIAL_H
#define GL_ENGINE_SHADING_MATERIAL_H

#include "Shader.h"

namespace gl_engine
{
	/*

		A material node links a shader to various textures and other attributs

	*/
	struct Material : public Shader
	{
		// // ----- CONSTRUCTOR ----- // //
	public:
		Material() {};
		Material(const std::string name, const char* vertexShader, const char* fragmentShader);

	};
} // namespace gl_engine
#endif
