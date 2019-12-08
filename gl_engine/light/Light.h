#ifndef GL_ENGINE_LIGHT_LIGHT_H
#define GL_ENGINE_LIGHT_LIGHT_H

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <../mesh/Mesh.h>
#include <../shading/Shader.h>
#include <../shading/Material.h>

/*

	Base light class. Designed to be extended by specific light types

*/
struct Light
{
	static const std::string LIGHT_COLOR;
	static const std::string LIGHT_BRIGHTNESS;

	// // ----- MEMBER VARIABLES ----- // //
private:
	GLfloat m_brightness = 1.0f;
	glm::vec3 m_color = { 1.0f, 1.0f, 1.0f };
	bool m_mesh_enabled = true;

	// // ----- CONSTRUCTORS ----- // //
public:
	Light() {};
	Light(const GLfloat brightness, const glm::vec3& color);


	// // ----- GETTERS ----- // //
	const GLfloat& brightness() const;
	const glm::vec3& color() const;
	
	virtual Mesh* mesh_ptr();
	virtual Shader* shader_ptr();

	// // ----- SETTERS ----- // //
	void setBrightness(const GLfloat brightness);
	void setColor(const glm::vec3 color);
	void enable_mesh();
	void disable_mesh();
};


#endif