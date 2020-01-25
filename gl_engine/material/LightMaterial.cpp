#include "LightMaterial.h"

namespace gl_engine
{

	LightMaterial::LightMaterial(const std::string& name, const char* vertex_shader, const char* fragment_shader) :
		Material{name, vertex_shader, fragment_shader}
	{}

	LightMaterial::LightMaterial(const std::string& name, const char* vertex_shader, const char* geometry_shader, const char* fragment_shader) :
		Material{ name, vertex_shader, geometry_shader, fragment_shader }
	{}

	void LightMaterial::init()
	{
		set_uniform(transform_model_to_projection, glm::mat4{ 1.0f });
		set_uniform(light_position, glm::vec3{ 0.0f });
		set_uniform(light_brightness, 0.0f);
		set_uniform(light_color, glm::vec3{ 0.0f })
	}

} // namespace gl_engine