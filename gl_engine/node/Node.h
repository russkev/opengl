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
		static const std::string U_MODEL_TO_PROJECTION;
		static constexpr auto POSITION_ATTR = 0u;
		inline static const auto MESH_VAO_INFO = gl_introspect_tuple<std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec2, GLint, glm::vec3, glm::vec3>>::get();
		static const glm::vec3 FORWARD_DIRECTION;

		// // ----- MEMBER VARIABLES ----- // //
	private:
		std::string m_name;
		glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };

		Node* m_parent = NULL;
		std::unordered_map<std::string, Node*> m_children;

		// // ----- CONSTRUCTOR ----- // //
	public:
		Node() {};
		Node(const std::string name);

		// // ----- GENERAL METHODS ----- // //
		void addChild(Node* child);
		void setParent(Node* parent);
		Node* disconnectChild(const std::string nodeName);

		virtual const glm::mat4 localTransform();
		const glm::mat4 worldTransform();
		const glm::mat3 worldNormalTransform();
		const glm::vec3 directionVector();

		virtual void update_view(CameraNode*);
		virtual void draw();

		// // ----- GETTERS ----- // //
		const std::string& name() const;

		virtual const glm::vec3 position() const;
		virtual const glm::vec3 worldPosition();
		const glm::vec3 rotation() const;
		const glm::vec3 scale() const;

		const Node* parent() const;
		std::unordered_map<std::string, Node*>& children();


		// // ----- SETTERS ----- // //
		virtual void setPosition(const glm::vec3&);
		virtual void setRotation(const glm::vec3&);
		virtual void setScale(const glm::vec3&);
		virtual void setScale(const GLfloat scale);


	private:
		void addParent(Node* parent);
	};
} // namespace gl_engine

#endif
