#include <algorithm>

#include <glm/gtx/transform.hpp>

#include "PointLight.h"


// // ----- CONSTRUCTOR ----- // //
// Constructor
Light::Light(const float brightness, const float radius, const glm::vec3& position) :
	m_brightness(brightness > 0? brightness : 0), 
	m_radius(radius > 0? radius : 0), 
	m_position(position),
	m_lightMesh(Sphere::createSphere(radius))
{}


// // ----- GETTERS ----- // //
const float& Light::brightness() const
{
	return m_brightness;
}

const float& Light::radius() const
{
	return m_brightness;
}

const glm::vec3& Light::position() const
{
	return m_position;
}
const glm::mat4& Light::transformMatrix() const
{
	return glm::translate(glm::mat4(), m_position);
}

// // ----- SETTERS ----- // //
float Light::brightness()
{
	return m_brightness;
}

float Light::radius()
{
	return m_radius;
}

glm::vec3& Light::position()
{
	return m_position;
}

void Light::transform(const glm::mat4& transformMatrix)
{
	m_position = { transformMatrix[3][0], transformMatrix[3][1], transformMatrix[3][2] };
}