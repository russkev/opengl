#ifndef GL_ENGINE_NODE_NODE_H
#define GL_ENGINE_NODE_NODE_H

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "../GL_Tuple_Introspect.h"

namespace gl_engine
{
	// // ----- FORWARD DECLERATION ----- // //
	struct CameraNode;

	/*

		Base node class. All other nodes derive from this. It can also be used as a transform node itself.

	*/
	struct Node
	{
		enum Pass
		{
			rgb,
			shadow
		};

		static const std::string U_MODEL_TO_PROJECTION;
		static constexpr auto POSITION_ATTR = 0u;
		inline static const auto MESH_VAO_INFO = gl_introspect_tuple<
			std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec2, GLint, glm::vec3, glm::vec3>>::get();
		static const glm::vec3 FORWARD_DIRECTION;

		// // ----- MEMBER VARIABLES ----- // //
	private:
		std::string m_name;
		glm::vec3 m_rotation{ 0.0f, 0.0f, 0.0f };
		glm::mat4 m_local_transform{ 1.0f };

		Node* m_parent = NULL;
		std::unordered_map<std::string, Node*> m_children;

		// // ----- CONSTRUCTOR ----- // //
	public:
		Node() {};
		Node(const std::string name);

		// // ----- GENERAL METHODS ----- // //
		void add_child(Node* child);
		void set_parent(Node* parent);
		Node* disconnect_child(const std::string nodeName);

		glm::mat4& local_to_node();
		const glm::mat4 world_to_node();
		const glm::mat3 world_normal_to_node();
		const glm::vec3 direction();

		virtual void update_view(CameraNode*);
		virtual void draw(const Pass& pass = rgb);

		// // ----- GETTERS ----- // //
		const std::string& name() const;

		virtual const glm::vec3 local_position() const;
		virtual const glm::vec3 world_position();
		const glm::vec3 local_rotation() const;
		const glm::vec3 local_scale() const;

		const Node* parent() const;
		std::unordered_map<std::string, Node*>& children();


		// // ----- SETTERS ----- // //
		virtual void set_position(const glm::vec3&);
		virtual void set_rotation(const glm::vec3&);
		virtual void set_scale(const glm::vec3&);
		virtual void set_scale(const GLfloat scale);


	private:
		void addParent(Node* parent);
	};
} // namespace gl_engine

#endif
