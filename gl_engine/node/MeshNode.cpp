#include "MeshNode.h"
#include "../GL_Tuple_Introspect.h"


/*

	Constructor
	Set member variables and upload mesh vertices and indices buffers

*/
MeshNode::MeshNode(const std::string name, Mesh* mesh, Material* material) :
	Node(name), m_mesh(mesh), m_material(material)
{
	m_vertexBuffer.append(m_mesh->vertices());
	m_indexBuffer.append(m_mesh->indices());

	// // Upload the VAO information
	m_vao.GenerateVAO(m_vertexBuffer, 0, MESH_VAO_INFO.data(), MESH_VAO_INFO.data() + MESH_VAO_INFO.size(), POSITION_ATTR);
}


void MeshNode::updateView(Camera* camerNode)
{
	glm::mat4 modelToPerspectiveMatrix = camerNode->worldToProjectionMatrix() * Node::worldTransform();
	m_material->setUniform(MODEL_TO_PROJECTION_UNIFORM_NAME, modelToPerspectiveMatrix);

}

void MeshNode::draw()
{
	m_material->use();
	m_vao.Bind();
	m_indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indexBuffer.size(), GL_UNSIGNED_SHORT, 0);
}




