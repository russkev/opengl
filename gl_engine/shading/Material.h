#ifndef GL_ENGINE_SHADING_MATERIAL_H
#define GL_ENGINE_SHADING_MATERIAL_H

#include "Shader.h"

/*

	A material node links a shader to various textures and other attributs

*/
struct Material : public Shader
{
	// // ----- MEMBER ATTRIBUTES ----- // //
//private:
//	Shader m_shader;
//
	// // ----- CONSTRUCTOR ----- // //
public:
	Material() {};
	Material(const std::string name, const char* vertexShader, const char* fragmentShader);
//
//	// // ----- GETTER ----- // //
//	Shader& shader();
};

#endif
