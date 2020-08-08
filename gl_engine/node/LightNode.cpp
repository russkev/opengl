#include "pch.h"
#include "LightNode.h"

#include "light/Light.h"
#include "shading/ShadowMap.h"
#include "CameraNode.h"

namespace glen
{
	// // ----- CONSTANTS ----- // //
	const std::string LightNode::LIGHT_POSITION = "position";
	const std::string LightNode::LIGHT_DIRECTION = "direction";

	// // ----- CONSTRUCTORS ----- // //
	LightNode::LightNode(const std::string name, Light* light) :
		Node{ name },
		m_light{ light },
		m_shader_pos{ 0 }
	{
		m_vertex_buffer.append(*m_light->mesh()->vertices());
		m_index_buffer.append(*m_light->mesh()->indices());

		// // Upload the VAO information
		m_vao.generate_VAO(m_vertex_buffer, 0, MESH_VAO_INFO.data(), MESH_VAO_INFO.data() + MESH_VAO_INFO.size(), POSITION_ATTR);
	}

	// // ----- GENERAL ----- // //
	void LightNode::update_view(CameraNode* cameraNode)
	{
		m_light->material()->update_view(cameraNode, this);
		Node::update_view(cameraNode);
	}

	void LightNode::draw()
	{

		if (m_light->material() == NULL)
		{
			if (!m_shader_warned)
			{
				printf("WARNING \"%s\" does not have a shader associated with it. Light mesh will not be rendered", Node::name().c_str());
				m_shader_warned = true;
			}
			return;
		}
		m_light->material()->use();

		m_vao.bind();
		m_index_buffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, (GLsizei)m_index_buffer.size(), GL_UNSIGNED_SHORT, 0);

		for (auto child : Node::children())
		{
			child.second->draw();
		}
	}

	// // ----- GETTERS ----- // //
	const Light* LightNode::light() const
	{
		return m_light;
	}

	Light* LightNode::light()
	{
		return m_light;
	}

	ShadowMap* LightNode::shadowMap()
	{
		return m_shadow_map;
	}

	const GLuint LightNode::shader_pos() const
	{
		return m_shader_pos;
	}


	// // ----- SETTERS ----- // //
	void LightNode::set_shadowMap(ShadowMap* shadowMap)
	{
		m_shadow_map = shadowMap;
	}

	void LightNode::set_shader_pos(const GLuint index)
	{
		m_shader_pos = index;
	}

}