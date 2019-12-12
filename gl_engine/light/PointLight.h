#pragma once

#include <glm/glm.hpp>

#include "Light.h"

#include "../shading/LoadShader.h"
#include "../mesh/Sphere.h"
#include "../mesh/Cube.h"

/*
	Emits light in all directions.
	Uses sphere mesh to display location and size in scene
*/
struct PointLight : public Light
{
	static const std::string LIGHT_COLOR;
	static const std::string LIGHT_BRIGHTNESS;
	static const std::string TYPE;

	// // ----- MEMBER VARIABLES ----- // //
private:
	GLfloat m_brightness = 1.0f;
	glm::vec3 m_color = { 1.0f, 1.0f, 1.0f };
	float m_radius = 1.0f;
	Mesh m_light_mesh;
	Shader m_shader;


	

	// // ----- CONSTRUCTORS ----- // //
public:
	PointLight(const GLfloat brightness, const glm::vec3 color);

	// // ----- GETTERS ----- // //
	const GLfloat& brightness() const override;
	const glm::vec3& color() const override;
	const float& radius() const;

	const std::string& type() const override;
	//std::vector<std::string> shader_commands() override;
	//void update_shader_commands() override;

	Mesh* mesh() override;
	Shader* shader() override;

	// // ----- SETTERS ----- // //	

	/*
	
		!!! Set brightness and color could probably be handled by parent

	*/
	void setBrightness(const GLfloat brightness) override;
	void setColor(const glm::vec3 color) override;
	void setRadius(const GLfloat radius);

};


struct PointLight_ShaderData
{
	std::pair<std::string, glm::vec3> position;
};