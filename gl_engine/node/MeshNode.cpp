#include "MeshNode.h"
#include "../GL_Tuple_Introspect.h"

#include "../Timer.h"


//Set member variables and upload mesh vertices and indices buffers
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
	Timer timer;
	//timer.debug_update();
	//m_material->use();
	//timer.debug_update();
	//glUseProgram(3);
	timer.update();
	m_vao.Bind();
	//timer.debug_update();

	m_indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
	//timer.debug_update();

	glDrawElements(GL_TRIANGLES, (GLsizei)m_indexBuffer.size(), GL_UNSIGNED_SHORT, 0);
	//timer.debug_update();

}


Mesh* MeshNode::mesh()
{
	return m_mesh;
}

Material* MeshNode::material()
{
	return m_material;
}




