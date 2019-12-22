#include "LightNode.h"
#include "../light/Light.h"
#include "../shading/ShadowMap.h"
#include "CameraNode.h"

namespace gl_engine
{
	const std::string LightNode::LIGHT_POSITION = ".position";
	const std::string LightNode::LIGHT_DIRECTION = ".direction";

	LightNode::LightNode(const std::string name, Light* light) :
		Node(name),
		m_light(light)
	{
		m_vertexBuffer.append(m_light->mesh()->vertices());
		m_indexBuffer.append(m_light->mesh()->indices());

		// // Upload the VAO information
		m_vao.GenerateVAO(m_vertexBuffer, 0, MESH_VAO_INFO.data(), MESH_VAO_INFO.data() + MESH_VAO_INFO.size(), POSITION_ATTR);
	}

	// // ----- GENERAL ----- // //
	void LightNode::set_shadowMap(ShadowMap* shadowMap)
	{
		m_shadowMap = shadowMap;

		m_shadowMap->setLightNode(this);
	}

	void LightNode::update_view(CameraNode* cameraNode)
	{
		m_modelToPerspectiveMatrix = cameraNode->worldToProjection_matrix() * Node::worldTransform();
		for (auto child : Node::children())
		{
			child.second->update_view(cameraNode);
		}
	}

	void LightNode::draw()
	{
		if (m_light->isEnabled())
		{
			if (m_light->shader() == NULL)
			{
				if (!m_shader_warned)
				{
					printf("WARNING \"%s\" does not have a shader associated with it. Light mesh will not be rendered", Node::name().c_str());
					m_shader_warned = true;
				}
				return;
			}
			m_light->shader()->setUniform(U_MODEL_TO_PROJECTION, m_modelToPerspectiveMatrix);
			m_light->shader()->use();

			m_vao.Bind();
			m_indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
			glDrawElements(GL_TRIANGLES, (GLsizei)m_indexBuffer.size(), GL_UNSIGNED_SHORT, 0);
		}

		for (auto child : Node::children())
		{
			child.second->draw();
		}
	}

	// // ----- GETTERS ----- // //
	Light* LightNode::light()
	{
		return m_light;
	}

	ShadowMap* LightNode::shadowMap()
	{
		return m_shadowMap;
	}
} // namespace gl_engine