#ifndef GL_ENGINE_LIGHT_LIGHT_H
#define GL_ENGINE_LIGHT_LIGHT_H

#pragma once
#include <vector>

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

	// // ----- MEMBER VARIABLES ----- // //
private:
	bool m_mesh_enabled = true;

	// // ----- CONSTRUCTORS ----- // //
public:
	Light() {};
	//Light(const GLfloat brightness, const glm::vec3& color);

	// // ----- GETTERS ----- // //
	virtual const GLfloat& brightness() const = 0;
	virtual const glm::vec3& color() const = 0;
	const bool& isEnabled() const;
	virtual Mesh* mesh();
	virtual Shader* shader();
	
	// // ----- SHADER COMMANDS ----- // //
	virtual const std::string& type() const = 0;
	//virtual std::vector<std::string> shader_commands() = 0;
	//virtual void update_shader_commands() = 0;

	// // ----- SETTERS ----- // //
	virtual void setBrightness(const GLfloat brightness) = 0;
	virtual void setColor(const glm::vec3 color) = 0;
	void enable_mesh();
	void disable_mesh();
};


#endif