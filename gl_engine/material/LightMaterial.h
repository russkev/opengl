#ifndef GL_ENGINE_MATERIAL_MATERIAL_H
#define GL_ENGINE_MATERIAL_MATERIAL_H

#include "Material.h"

#include <tuple>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

namespace gl_engine
{
	template<typename T>
	struct Property
	{
		const std::string name;
		T value;
	};

	struct LightMaterial : public Material
	{
		void init();

	private:
		std::vector<Property> m_properties
		{
			{ "transfom.model_to_projection", glm::mat4{1.0f} },
			{ "light.position", glm::vec3{0.0f} },
			{ "light.brightness", 1.0f },
			{ "light.color", glm::vec3{1.0f} }
		}
	};
} // namespace gl_engine


#endif