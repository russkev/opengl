#ifndef GL_ENGINE_LIGHT_POINT_H
#define GL_ENGINE_LIGHT_POINT_H
						
#include <glm/glm.hpp>

#include "Light.h"

#include "../shading/LoadShader.h"
#include "../mesh/Sphere.h"


/*

	Emits light in all directions.
	Uses sphere mesh to display location and size in scene

*/
struct PointLight : public Light
{
	static const std::string TYPE;

	// // ----- MEMBER VARIABLES ----- // //
private:
	float m_radius = 0.5f;
	Mesh m_light_mesh;
	Shader m_shader;

	// // ----- CONSTRUCTORS ----- // //
public:
	PointLight(const GLfloat brightness, const glm::vec3 color);

	// // ----- GETTERS ----- // //
	const float& radius() const;

	const std::string& type() const override;

	Mesh* mesh() override;
	Shader* shader() override;

	// // ----- SETTERS ----- // //	
	void setRadius(const GLfloat radius);

};

#endif