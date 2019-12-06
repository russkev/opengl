#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include "PointLight.h"



// // ----- CONSTRUCTORS ----- // //

PointLight::PointLight(const GLfloat brightness, const glm::vec3 color, const float radius) :
	Light(brightness, color),
	m_radius(radius > 0 ? radius : 0.1f)
{}


// // ----- GETTERS ----- // //
const float& PointLight::radius() const
{
	return m_radius;
}

const Mesh* PointLight::mesh_ptr() const
{
	return &m_lightMesh;
}
const Shader* PointLight::shader_ptr() const
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