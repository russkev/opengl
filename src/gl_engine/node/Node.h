#ifndef GL_ENGINE_NODE_NODE_H
#define GL_ENGINE_NODE_NODE_H

#include "resources/GL_Tuple_Introspect.h"

namespace glen
{
	// // ----- FORWARD DECLERATION ----- // //
	struct CameraNode;
	struct Material;

	/*!

		Base node class. All other nodes derive from this. 
		It can be used as a parent transform for other Nodes.
		Can be instantiated as a 'dummy' node.

	*/
	struct Node
	{
		// @cond

		// // ----- ENUMERATIONS ----- // //
		enum Pass
		{
			rgb,
			shadow
		};


		// // ----- CONSTANTS ----- // //
		static const std::string U_MODEL_TO_PROJECTION;
		static constexpr auto POSITION_ATTR = 0u;
		const std::array<member_info_type, 7> MESH_VAO_INFO = gl_introspect_tuple<std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec2, GLint, glm::vec3, glm::vec3>>::get();

		static const glm::vec3 FORWARD_DIRECTION;
		
		// @endcond

		// // ----- CONSTRUCTORS ----- // //
	public:
		/*!
		 * @brief Constructor.
		*/
		Node() {};

		/*!
		 * @brief Constructor.
		*/
		Node(const std::string name);

		// // ----- GENERAL METHODS ----- // //

		/*!
		 * @brief If node type has a material, update it with the latest 
		 * camera position.If node has children, update them aswell.
		*/
		virtual void update_view(CameraNode*);

		/*!
		 * @brief Render the mesh to the screen (if applicable).
		*/
		virtual void draw();

		/*!
		 * @brief Render mesh with provided `material` (if applicable).
		*/
		virtual void draw_material(Material* material);


		
		// // ----- GETTERS ----- // //

		/*!
		 * @return The node name
		*/
		const std::string& name() const;

		/*!
		 * @return Transform from the parent node to this one.
		*/
		const glm::mat4 local_to_node() const;
		
		/*!
		 * @return Transform matrix from the parent node to this one (mutable).
		*/
		glm::mat4* local_to_node();

		/*!
		 * @return The transform matrix from the world to this node.
		*/
		const glm::mat4 world_to_node() const;

		/*!
		 * @return The transform matrix normals from world to this node.
		 * This is useful for doing deferrend rendering with a g-buffer.
		*/
		const glm::mat3 world_normal_to_node() const;

		/*!
		 * @return The direction the node is pointing.
		*/
		const glm::vec3 direction();

		/*!
		 * @return The local coordinates.
		*/
		virtual const glm::vec3 local_position() const;

		/*!
		 * @return The world space coordinates.
		*/
		virtual const glm::vec3 world_position() const;

		/*!
		 * @return The local rotation (Euler angles).
		*/
		const glm::vec3 local_rotation() const;

		/*!
		 * @brief The local scale values.
		 * @return 
		*/
		const glm::vec3 local_scale() const;

		/*!
		 * @return The parent of this node.
		*/
		const Node* parent() const;

		/*!
		 * @return The children of this node (mutable). 
		*/
		std::unordered_map<std::string, Node*>& children();


		// // ----- SETTERS ----- // //
		
		/*!
		 * @brief Add a node as a child of this one.
		 * 
		 * All children will transform with their parents.
		*/
		void add_child(Node* child);

		/*!
		 * @brief Set which node will be the parent of this one.
		*/
		void set_parent(Node* parent);

		/*!
		 * @brief Disconnect the given child node. 
		 * 
		 * @return The disconnected node (if it exists as a child)
		*/
		Node* disconnect_child(const std::string nodeName);

		/*!
		 * @brief Set `position` coordinates of node
		*/
		virtual void set_position(const glm::vec3& position);

		/*!
		 * @brief Set `rotation` (Euler angles) of node 
		*/
		virtual void set_rotation(const glm::vec3& rotation);

		/*!
		 * @brief Set the scale values of the node
		*/
		virtual void set_scale(const glm::vec3& scale);

		/*!
		 * @brief Set a single value with which to scale the node uniformly
		*/
		virtual void set_scale(const GLfloat scale);

	private:
		void add_parent(Node* parent);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		std::string m_name;
		glm::vec3 m_rotation{ 0.0f, 0.0f, 0.0f };
		glm::mat4 m_local_transform{ 1.0f };

		Node* m_parent = NULL;
		std::unordered_map<std::string, Node*> m_children;
	};
}

#endif
