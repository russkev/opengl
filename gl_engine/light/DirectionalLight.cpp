#include "DirectionalLight.h"

const std::string DirectionalLight::TYPE = "light";

DirectionalLight::DirectionalLight(GLfloat brightness, glm::vec3 color) :
	m_light_mesh(Arrow::createArrow()),
	m_shader(Shader("lightShader", "lightShader.vert", "lightShader.frag"))
{
	setBrightness(brightness);
	setColor(color);
}

// // ----- GETTERS ----- // //
const std::string& DirectionalLight::type() const
{
	return TYPE;
}

Mesh* DirectionalLight::mesh()
{
	return &m_light_mesh;
}
Shader* DirectionalLight::shader()
{
	return &m_shader;
}