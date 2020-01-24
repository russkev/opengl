#include "LightMaterial.h"

namespace gl_engine
{
	void LightMaterial::init()
	{
		Material::init();
		for (const auto & property : m_properties)
		{
			set_uniform(property.name, property.value);
		}
	}
} // namespace gl_engine