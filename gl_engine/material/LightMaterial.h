#ifndef GL_ENGINE_MATERIAL_LIGHT_MATERIAL_H
#define GL_ENGINE_MATERIAL_LIGHT_MATERIAL_H

#include "Material.h"

#include <tuple>
#include <string>
#include <vector>
#include <map>
#include <set>

#include <glm/glm.hpp>

#include <GL/glew.h>

namespace gl_engine
{
	// Storing properties of multiple types by stackoverflow user sbi:
	// https://stackoverflow.com/questions/3559412/how-to-store-different-data-types-in-one-list-c
	struct Property
	{
		Property(const std::string& name) :
			m_name(name) {}
		virtual ~Property() {}
	private:
		const std::string m_name;
	};

	template<typename T>
	struct TypedProperty : public Property
	{
		TypedProperty(const std::string& name, const T& data) :
			Property(name), m_data(data) {}

	private:
		T m_data;
	};

	typedef std::shared_ptr<Property> property_type;
	//typedef std::vector<property_type> property_list_type;

	//struct PropertyList : public std::vector< std::shared_ptr<Property>>
	//{
	//	template <typename T>
	//	PropertyList(TypedProperty<T>& property)
	//	{
	//		push_back()
	//	}

	//	template<typename T, typename... Args>
	//	PropertyList(TypedProperty<T>& property, Args... args)
	//	{
	//		push_back(property.name(), property.type())
	//		{
	//			PropertyList(args...)
	//		}
	//	}


	//	template<typename T>
	//	void push_back(const std::string& name, const T& data) override
	//	{
	//		std::vector<std::shared_ptr<Property> >::push_back(
	//			std::shared_ptr<Property>{ new TypedProperty<T>{ name, data } }
	//		)
	//	}
	//};

	struct LightMaterialProperties
	{
		std::pair<std::string, glm::vec3> light_color{ "light.color", glm::vec3{1.0f} };

	};


	struct LightMaterial : public Material
	{
		inline static const std::string transform_model_to_projection = "transform.model_to_projection";
		inline static const std::string light_position = "light.position";
		inline static const std::string light_brightness = "light.brightness";
		inline static const std::string light_color = "light.color";


		LightMaterial(const std::string& name, const char* vertex_shader, const char* fragment_shader);
		LightMaterial(const std::string& name, const char* vertex_shader, const char* geometry_shader, const char* fragment_shader);
	private:
		void init();

	private:
		LightMaterialProperties m_properties;
	};
} // namespace gl_engine


#endif