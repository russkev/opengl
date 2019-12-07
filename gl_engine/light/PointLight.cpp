#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include "PointLight.h"


// // ----- STATICS ----- // //

//Shader PointLight::m_shader = Shader("lightShader", "LightMesh.vert", "LightMesh.frag");

// // ----- CONSTRUCTORS ----- // //

PointLight::PointLight(const GLfloat brightness, const glm::vec3 color, const GLfloat radius) :
	Light(brightness, color),
	m_radius(radius > 0 ? radius : 0.1f),
	m_light_mesh(Sphere::createSphere(m_radius))
{
	m_shader = Shader("lightShader", "lightShader.vert", "lightShader.frag");
}


// // ----- GETTERS ----- // //
const float& PointLight::radius() const
{
	return m_radius;
}

Mesh* PointLight::mesh_ptr()
{
	return &m_light_mesh;
}
Shader* PointLight::shader_ptr()
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