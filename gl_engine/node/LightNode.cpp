#include "LightNode.h"
#include "../light/Light.h"

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

// // ----- SHADOW MAP ----- // //
void LightNode::init_shadowMap()
{
	// Create frame buffer object
	glGenFramebuffers(1, &m_depthMap_FBO);

	// Create 2D texture
	//const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	glGenTextures(1, &m_depthMap_ID);
	glBindTexture(GL_TEXTURE_2D, m_depthMap_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Attach texture to framebuffer's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap_ID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightNode::render_shadowMap()
{
	// 1. Render depth map
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap_FBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Configure shader and matrices
	GLfloat near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(
		glm::vec3(-2.0f, 4.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	// !!! Render fro m light perspective


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. Render
}

// // ----- GENERAL ----- // //
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