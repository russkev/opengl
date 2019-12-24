#ifndef GL_ENGINE_SHADING_MATERIAL_H
#define GL_ENGINE_SHADING_MATERIAL_H

#include <map>
#include <GL/glew.h>
#include <string>

#include "Shader.h"



namespace gl_engine
{
	// // ----- FORWARD DECLERATION ----- // //
	struct Texture;

	/*

		A material node links a shader to various textures and other attributs

	*/
	struct Material : public Shader
	{
		std::map<std::string, Texture*> m_textures;

		// // ----- CONSTRUCTOR ----- // //
	public:
		Material() {};
		Material(const std::string name, const char* vertexShader, const char* fragmentShader);

		// // ----- GENERAL ----- // //
		void addTexture(const std::string& uniform_name, Texture* texture);
		void bindTextures();
		void unbindTextures();

	};
} // namespace gl_engine
#endif
