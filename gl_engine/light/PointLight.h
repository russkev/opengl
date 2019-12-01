#pragma once

#include <glm/glm.hpp>

#include "../shading/LoadShader.h"
#include "../mesh/Sphere.h"
#include "../mesh/Cube.h"

/*
	Emits light in all directions.
	Uses sphere mesh to display location and size in scene
*/

struct PointLight
{
	// // ----- CONSTRUCTORS ----- // //
	PointLight();
	PointLight(const glm::vec3& position);
	PointLight(const float brightness, const glm::vec3& position);
	PointLight(const float brightness, const float radius, const glm::vec3& position);
	PointLight(const float brightness, const float radius, const glm::vec3& position, const bool displayMesh); 

	// // ----- GETTERS ----- // //
	const float& brightness() const;
	const float& radius() const;
	const glm::vec3& position() const;
	const glm::mat4 transformMatrix() const;
	const Mesh& mesh() const;
	const GLuint& programID() const;

	// // ----- SETTERS ----- // //
	float& brightness();
	float& radius();
	glm::vec3& position();
	void transform(const glm::mat4&);
	Mesh& mesh();
	GLuint& programID();


private:
	float m_brightness		= 1.0f;
	float m_radius			= 1.0f;
	glm::vec3 m_position	= { 0.0f, 0.0f, 0.0f };
	bool m_displayMesh		= true;
	Mesh m_lightMesh		= Cube::createCube();
	GLuint m_programID;
};