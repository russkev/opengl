#ifndef GL_ENGINE_LIGHT_DIRECTIONAL_H
#define GL_ENGINE_LIGHT_DIRECTIONAL_H

#include <glm/glm.hpp>

#include "Light.h"

#include "../mesh/Arrow.h"
#include "../shading/LoadShader.h"

/*

	Emits light in a single direction. Rays are parallel like the sun.

*/
struct DirectionalLight : public Light
{
private:
	static const std::string TYPE;
	//static const std::string LIGHT_COLOR;
	//static const std::string LIGHT_BRIGHTNESS;

	// // ----- MEMBER VARIABLES ----- // //
private:
	//GLfloat m_brightness = 1.0f;
	//glm::vec3 m_color = { 1.0f, 1.0f, 1.0f };
	Mesh m_light_mesh;
	Shader m_shader;

	// // ----- CONSTRUCTORS ----- // //
public:
	DirectionalLight(GLfloat brightness, glm::vec3 color);

	// // ----- GETTERS ----- // //
	//const GLfloat& brightness() const override;
	//const glm::vec3& color() const override;
	const std::string& type() const override;

	Mesh* mesh() override;
	Shader* shader() override;

	// // ----- SETTERS ----- // //	
	//void setBrightness(const GLfloat brightness) override;
	//void setColor(const glm::vec3 color) override;
};
#endif