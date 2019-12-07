#include "MeshNode.h"

#include "../Timer.h"

const std::string MeshNode::U_MODEL_TO_WORLD = "mat_modelToWorld";

//Set member variables and upload mesh vertices and indices buffers
MeshNode::MeshNode(const std::string name, Mesh* mesh, Material* material) :
	Node(name), m_mesh(mesh), m_material(material)
{
	m_vertexBuffer.append(m_mesh->vertices());
	m_indexBuffer.append(m_mesh->indices());

	// // Upload the VAO information
	m_vao.GenerateVAO(m_vertexBuffer, 0, MESH_VAO_INFO.data(), MESH_VAO_INFO.data() + MESH_VAO_INFO.size(), POSITION_ATTR);
}


void MeshNode::update_view(Camera* camera)
{
	glm::mat4 modelToPerspectiveMatrix = camera->worldToProjectionMatrix() * Node::worldTransform();
	m_material->setUniform(U_MODEL_TO_PROJECTION, modelToPerspectiveMatrix);
	m_material->setUniform(U_MODEL_TO_WORLD, Node::worldTransform());

	for (auto child : Node::children())
	{
		child.second->update_view(camera);
	}

}

void MeshNode::draw()
{
	m_material->use();
	m_vao.Bind();
	m_indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indexBuffer.size(), GL_UNSIGNED_SHORT, 0);

	for (auto child : Node::children())
	{
		child.second->draw();
	}

}


Mesh* MeshNode::mesh()
{
	return m_mesh;
}

Material* MeshNode::material()
{
	return m_material;
}




