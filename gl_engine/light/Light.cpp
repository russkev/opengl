#include "Light.h"

Light::Light(const GLfloat brightness, const glm::vec3& color) :
	m_brightness(brightness > 0 ? brightness : 0),
	m_color(color)
{}

// // ----- GETTERS ----- // //
const GLfloat& Light::brightness() const
{
	return m_brightness;
}

const glm::vec3& Light::color() const
{
	return m_color;
}

Mesh* Light::mesh_ptr()
{
	return NULL;
}

Shader* Light::shader_ptr() {
	return NULL;
}

// // ----- SETTERS ----- // //
void Light::setBrightness(const GLfloat brightness)
{
	m_brightness = brightness;
}
void Light::setColor(const glm::vec3 color)
{
	m_color = color;
}
void Light::enable_mesh()
{
	m_mesh_enabled = true;
}

void Light::disable_mesh()
{
	m_mesh_enabled = false;
}
