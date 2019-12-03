#pragma once

#include "Shader.h"

/*

	A material node links a shader to various textures and other attributs

*/
struct Material
{
	// // ----- MEMBER ATTRIBUTES ----- // //
private:
	Shader m_shader;

	// // ----- CONSTRUCTOR ----- // //
public:
	Material(const std::string name, const char* vertexShader, const char* fragmentShader);
};
