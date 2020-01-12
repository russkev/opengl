#ifndef GL_ENGINE_NODE_SCREEN_H
#define GL_ENGINE_NODE_SCREEN_H

#include <glm/glm.hpp>

#include "Node.h"
#include "../Buffer.h"
#include "../VAO.h"

namespace gl_engine
{
	// // ----- FORWARD DECLARATION ----- // //
	struct Mesh;
	/*

		2D node for elements that are intended to be displayed in screen space

	*/
	struct ScreenNode : public Node
	{
		// // ----- CONSTRUCTORS ----- // //
	public:
		ScreenNode() {};
		ScreenNode(const std::string name, Mesh* mesh, Material* material);

		// // ----- GENERAL METHODS ----- // //
		void update_view(CameraNode*) override {};
		void draw(const Pass& pass = rgb) override;
		void draw_material(Material* material) override;

		// // ----- GETTERS ----- // //
		const glm::vec3 local_position() const override;
		const glm::vec3 world_position() override;

		// // ----- SETTERS ----- // //
		void set_position(const glm::vec3&) override;
		void set_position(const glm::vec2&);
		void set_rotation(const glm::vec3&) override;
		void set_rotation(const glm::vec2&);
		void set_scale(const glm::vec3&) override;
		void set_scale(const glm::vec2&);
		void set_scale(const GLfloat scale);

		// // ----- MEMBER VARIABLES ----- // //
		Mesh* m_mesh;
		Material* m_material;
		Buffer m_vertex_buffer = { GL_ARRAY_BUFFER, 0 };
		Buffer m_index_buffer = { GL_ARRAY_BUFFER, 0 };
		VAO m_vao;
	};
} // namespace gl_engine
#endif