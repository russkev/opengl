#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include "PointLight.h"


// // ----- STATICS ----- // //
const std::string PointLight::TYPE = "light";

// // ----- CONSTRUCTORS ----- // //

PointLight::PointLight(const GLfloat brightness, const glm::vec3 color) :
	m_light_mesh(Sphere::createSphere(m_radius)),
	m_shader(Shader("lightShader", "lightShader.vert", "lightShader.frag"))
{
	setBrightness(brightness);
	setColor(color);
}


// // ----- GETTERS ----- // //
//const GLfloat& PointLight::brightness() const
//{
//	return m_brightness;
//}
//
//const glm::vec3& PointLight::color() const
//{
//	return m_color;
//}


const float& PointLight::radius() const
{
	return m_radius;
}

const std::string& PointLight::type() const
{
	return TYPE;
}

Mesh* PointLight::mesh()
{
	return &m_light_mesh;
}
Shader* PointLight::shader()
{
	return &m_shader;
}

// // ----- SETTERS ----- // //
void PointLight::setRadius(const GLfloat radius)
{
	if (radius < 0)
	{
		std::printf("WARNING: unable to set radius to %.3f, can't set to negative", radius);
	}
	else
	{
		m_radius = radius;
	}
}