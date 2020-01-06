#include "Light.h"

#include "../camera/FreeCamera.h"

namespace gl_engine
{
	// // ----- STATICS ----- // //
	const std::string Light::LIGHT_SHADER_TYPE = "light";
	const std::string Light::LIGHT_BRIGHTNESS = "brightness";
	const std::string Light::LIGHT_COLOR = "color";

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

	Shader* Light::shader()
	{
		return NULL;
	}

	const bool& Light::is_enabled() const
	{
		return m_mesh_enabled;
	}

	Camera* Light::camera()
	{
		return NULL;
	}


	// // ----- SETTERS ----- // //
	void Light::set_brightness(const GLfloat brightness)
	{
		m_brightness = brightness;
		if (shader() != NULL)
		{
			shader()->set_uniform(std::string(LIGHT_SHADER_TYPE + "." + LIGHT_BRIGHTNESS), m_brightness);
		}
	}

	void Light::set_color(const glm::vec3 color)
	{
		m_color = color;
		if (shader() != NULL)
		{
			shader()->set_uniform(std::string(LIGHT_SHADER_TYPE + "." + LIGHT_COLOR), m_color);
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
} // namespace gl_engine