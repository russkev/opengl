#include "LightNode.h"

LightNode::LightNode(const std::string name, Light* light) :
	Node(name),
	m_light(light)
{
	m_vertexBuffer.append(m_light->mesh_ptr()->vertices());
	m_indexBuffer.append(m_light->mesh_ptr()->indices());

	// // Upload the VAO information
	m_vao.GenerateVAO(m_vertexBuffer, 0, MESH_VAO_INFO.data(), MESH_VAO_INFO.data() + MESH_VAO_INFO.size(), POSITION_ATTR);
}

Light* LightNode::light()
{
	return m_light;
}

void LightNode::update_view(Camera* camera)
{
	m_modelToPerspectiveMatrix = camera->worldToProjection_matrix() * Node::worldTransform();
	for (auto child : Node::children())
	{
		child.second->update_view(camera);
	}
}

void LightNode::draw()
{
	m_light->shader_ptr()->setUniform(U_MODEL_TO_PROJECTION, m_modelToPerspectiveMatrix);

	m_light->shader_ptr()->use();
	m_vao.Bind();
	m_indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indexBuffer.size(), GL_UNSIGNED_SHORT, 0);

	for (auto child : Node::children())
	{
		child.second->draw();
	}
}