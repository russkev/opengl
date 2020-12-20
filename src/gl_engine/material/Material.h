#ifndef GL_ENGINE_MATERIAL_MATERIAL_H
#define GL_ENGINE_MATERIAL_MATERIAL_H

#include "ShaderType.h"
#include "shading/Texture.h"

namespace glen
{
	// // ----- FORWARD DECLERATIONS ----- // //
	struct Node;
	struct LightNode;
	struct CameraNode;
	struct Framebuffer;

	/*

		Helper struct to store uniform information

	*/
	struct Uniform
	{
		GLint location;
		GLenum type;
		GLsizei data_size;
		GLuint texture_unit;
	};



	/*!

		Main material class that stores relevent information and functions about a particular material.

	*/
	struct Material
	{


		// // ----- CONSTRUCTOR ----- // //
	public:
		/*!
		 * @brief Constructor.
		 * @param name Name to give material.
		 * @param vertex_shader Path to the vertex shader. Should be a `.vert` file.
		 * @param fragment_shader Path to the fragment shader. Should be a `.frag` file.
		*/
		Material(const std::string& name, const std::string& vertex_shader, const std::string& fragment_shader);

		/*!
		 * @brief Constructor.
		 * @param name Name to give material.
		 * @param vertex_shader Path to the vertex shader. Should be a `.vert` file.
		 * @param fragment_shader Path to the fragment shader. Should be a `.frag` file.
		*/
		Material(const std::string& name, const std::string& vertex_shader, const std::string& geometry_shader, const std::string& fragment_shader);

		/*!
		 * @brief Constructor.
		 * @param name Name to give material.
		 * @param vertex_shader Path to the vertex shader. Should be a `.vert` file.
		 * @param fragment_shader Path to the fragment shader. Should be a `.frag` file.
		*/
		Material(const std::string& name, const char* vertex_shader, const char* fragment_shader);

		/*!
		 * @brief Constructor.
		 * @param name Name to give material.
		 * @param vertex_shader Path to the vertex shader. Should be a `.vert` file.
		 * @param geometry_shader Path to the geometry shader. Should be a `.geom` file.
		 * @param fragment_shader Path to the fragment shader. Should be a `.frag` file.
		*/
		Material(const std::string& name, const char* vertex_shader, const char* geometry_shader, const char* fragment_shader);

		/*!
		 * @brief Use move instead constructor instead of copy constructor.
		*/
		Material(const Material& other) = delete;

		/*!
		 * @brief Move constructor.
		*/
		Material(Material&& other) noexcept;

		/*!
		 * @brief Move assign.
		*/
		Material& operator = (Material&& other) noexcept;

		/*!
		* @brief Destructor.
		*/
		~Material();

		
		// // ----- GENERAL METHODS ----- // //
		
		/*!
		 * @brief Use this to get a path to the shaders directory that works with unit tests.
		 * 
		 * This relies on there being a preprocessor directive.
		 * In Visual Studio 2019, this setting can by right clicking on the `gl_engine` project and
		 * clicking `Configuration` -> `C/C++` -> `Preprocessor`.
		 * @return Absolute path to shaders directory.
		*/
		static std::string shaders_dir();

		/*!
		 * @brief Tell OpenGL to use the shader program for this shader.
		*/
		void use();

		/*!
		 * @return True if shader program contains the specified uniform.
		*/
		bool contains_uniform(const std::string uniform_name);

		/*!
		 * @brief Iterate over all lights in `light_nodes` and update the material.
		 * @param light_nodes The lights to iterate over.
		*/
		virtual void update_lights(const std::vector<LightNode*>& light_nodes) {}

		/*!
		 * @brief Update shader with new light position.
		 * @param light_node The light to update with.
		 * @param camera_node The main viewing camera.
		*/
		virtual void update_light_transform(const LightNode* light_node, const CameraNode* camera_node) {};

		/*!
		 * @brief Ensure all textures associated with this material are 'active'
		 * in relation to OpenGL.
		*/
		void bind_textures();

		/*!
		 * @brief Unbind textures.
		*/
		void unbind_textures();

		/*!
		 * @brief Update material with new camera position.
		 * @param cameraNode The main viewing camera.
		 * @param node The node (usually a `MeshNode` for which the material is applied.
		*/
		virtual void update_view(const CameraNode* cameraNode, const Node* node) {};

	private:
		void init();
		bool is_uniform(const GLenum type);
		void update_texture_id(const std::string uniform_name, const GLuint id);
		void bind_texture(const std::string uniform_name, Texture* texture);
		void fetch_uniforms();


		// // ----- GETTERS ----- // //
	public:
		/*!
		 * @return The shader program ID.
		*/
		const GLuint program_id() const;

		/*!
		 * @return The name of the shader.
		*/
		const std::string name() const;

		/*!
		 * @return All uniforms of the shader associated with this material.
		*/
		const std::map<std::string, Uniform>& uniforms() const;

		/*!
		 * @return The number of uniforms in the shader associated with this material.
		*/
		const GLuint num_uniforms() const;

		/*!
		 * @return The number of texture slots available in the shader associated with this material.
		*/
		const GLuint num_textures() const;

		
		// // ----- SETTERS ----- // //

		/*!
		 * @param name The new name for the shader.
		*/
		void set_name(const std::string& name);

		/*!
		 * @brief Provide a color to the shader in place of a texture.
		 * @param uniform_name The name of the uniform.
		 * @param color The r, g, b color. Values in the range 0 to 1.
		*/
		void set_sampler_color(const std::string& uniform_name, const glm::vec3& color);
		void set_sampler_value(const std::string& uniform_name, const GLfloat value);
		void set_texture(const std::string& uniform_name, Texture* texture);


		/*!
		 * @brief Set uniforms of the shader
		 * 
		 * Automatically choose correct upload path based on data type
		 * Defined in header because of template usage
		 * @tparam T A type accepted by OpenGL shaders
		 * @param name The name of the uniform
		 * @param data_vector The data to send to the uniform
		*/
		template<typename T>
		void set_uniform(const std::string& name, const std::vector<T>& data_vector)
		{
			set_uniform(name + "[0]", data_vector[0], (GLuint)data_vector.size());
		}

		/*!
		 * @brief Set uniforms of the shader
		 *
		 * Automatically choose correct upload path based on data type
		 * Defined in header because of template usage
		 * @tparam T A type accepted by OpenGL shaders
		 * @param name The name of the uniform
		 * @param data The data to send to the uniform
		 * @param num_elements The number of elements to send to the uniform
		*/
		template<typename T>
		void set_uniform(const std::string& name, const T& data, const GLuint num_elements = 1u)
		{
			GLenum dataEnumType = ShaderType::gl_types.at(&typeid(T));
			Uniform* thisUniform;
			std::string name_s = name;

			if (m_uniforms.size() == 0)
			{
				if (m_failed_uniforms.find(name_s) == m_failed_uniforms.end())
				{
					printf("WARNING: No uniforms stored in \"%s\". \"%s\" will not be set\n", m_name.c_str(), name.c_str());
					m_failed_uniforms.insert(name_s);
				}
				return;
			}
			try
			{
				thisUniform = &m_uniforms.at(name);
			}
			catch (std::out_of_range)
			{
				if (m_failed_uniforms.find(name_s) == m_failed_uniforms.end())
				{
					printf("WARNING: Setting uniform \"%s\" for shader \"%s\" failed. Uniform not found\n", name_s.c_str(), m_name.c_str());
					m_failed_uniforms.insert(name_s);
				}
				return;
			}
			use();
			upload_uniform(thisUniform->location, num_elements, data);
		}

		// // ----- MEMBER VARIABLES ----- // //
	private:
		std::string m_name;
		GLuint m_program_id = 0;

		std::map<std::string, Uniform> m_uniforms;
		std::map<std::string, Texture*> m_textures;

		std::set<std::string> m_failed_uniforms;
		std::set<std::string> m_failed_textures;

		GLuint m_num_uniforms = 0;
		std::list<Texture> m_colors;

		// // ----- UNIFORM UPLOAD METHODS ----- // //
	private:
		void upload_uniform(const GLint loc, const GLuint num, const GLfloat& data) { num == 1 ? glUniform1f(loc, data) : glUniform1fv(loc, num, &data); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::vec2& data) { glUniform2fv(loc, num, &data[0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::vec3& data) { glUniform3fv(loc, num, &data[0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::vec4& data) { glUniform4fv(loc, num, &data[0]); };
		void upload_uniform(const GLint loc, const GLuint num, const GLdouble& data) { num == 1 ? glUniform1d(loc, data) : glUniform1dv(loc, num, &data); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::dvec2& data) { glUniform2dv(loc, num, &data[0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::dvec3& data) { glUniform3dv(loc, num, &data[0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::dvec4& data) { glUniform4dv(loc, num, &data[0]); };
		void upload_uniform(const GLint loc, const GLuint num, const GLint& data) { num == 1 ? glUniform1i(loc, data) : glUniform1iv(loc, num, &data); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::ivec2& data) { glUniform2iv(loc, num, &data[0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::ivec3& data) { glUniform3iv(loc, num, &data[0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::ivec4& data) { glUniform4iv(loc, num, &data[0]); };
		void upload_uniform(const GLint loc, const GLuint num, const GLuint& data) { num == 1 ? glUniform1i(loc, data) : glUniform1uiv(loc, num, &data); }; // Doesn't like glUniform1ui
		void upload_uniform(const GLint loc, const GLuint num, const glm::uvec2& data) { glUniform2uiv(loc, num, &data[0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::uvec3& data) { glUniform3uiv(loc, num, &data[0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::uvec4& data) { glUniform4uiv(loc, num, &data[0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::mat2& data) { glUniformMatrix2fv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::mat3& data) { glUniformMatrix3fv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::mat4& data) { glUniformMatrix4fv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::mat2x3& data) { glUniformMatrix2x3fv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::mat2x4& data) { glUniformMatrix2x4fv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::mat3x2& data) { glUniformMatrix3x2fv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::mat3x4& data) { glUniformMatrix3x4fv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::mat4x2& data) { glUniformMatrix4x2fv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::mat4x3& data) { glUniformMatrix4x3fv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::dmat2& data) { glUniformMatrix2dv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::dmat3& data) { glUniformMatrix3dv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::dmat4& data) { glUniformMatrix4dv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::dmat2x3& data) { glUniformMatrix2x3dv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::dmat2x4& data) { glUniformMatrix2x4dv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::dmat3x2& data) { glUniformMatrix3x2dv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::dmat3x4& data) { glUniformMatrix3x4dv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::dmat4x2& data) { glUniformMatrix4x2dv(loc, num, false, &data[0][0]); };
		void upload_uniform(const GLint loc, const GLuint num, const glm::dmat4x3& data) { glUniformMatrix4x3dv(loc, num, false, &data[0][0]); };
	};	
}
#endif