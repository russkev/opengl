#include "Light.h"

const std::string Light::LIGHT_COLOR		= "light.color";
const std::string Light::LIGHT_BRIGHTNESS	= "light.brightness";

Light::Light(const GLfloat brightness, const glm::vec3& color)
{
	setBrightness(brightness);
	setColor(color);
}

// // ----- GETTERS ----- // //
const GLfloat& Light::brightness() const
{
	return m_brightness;
}

const glm::vec3& Light::color() const
{
	return m_color;
}

Mesh* Light::mesh()
{
	return NULL;
}

Shader* Light::shader() {
	return NULL;
}

const bool& Light::isEnabled() const
{
	return m_mesh_enabled;
}

// // ----- SETTERS ----- // //
void Light::setBrightness(const GLfloat brightness)
{
	m_brightness = brightness;
	if (shader() != NULL)
	{
		shader()->setUniform(LIGHT_BRIGHTNESS, m_brightness);
	}
}

void Light::setColor(const glm::vec3 color)
{
	m_color = color;
	if (shader() != NULL)
	{
		shader()->setUniform(LIGHT_COLOR, m_color);
	}
}

void Light::enable_mesh()
{
	m_mesh_enabled = true;
}

void Light::disable_mesh()
{
	m_mesh_enabled = false;
}
