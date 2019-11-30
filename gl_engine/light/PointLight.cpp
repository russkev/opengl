#include "PointLight.h"


Light::Light(const float brightness, const float radius, const glm::vec3& position) :
	m_brightness(brightness), m_radius(radius), m_position(position)
{
	m_lightMesh = Sphere::createSphere(m_radius);
}