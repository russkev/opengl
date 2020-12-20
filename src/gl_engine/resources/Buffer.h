#ifndef GL_ENGINE_BUFFER_H
#define GL_ENGINE_BUFFER_H

namespace glen
{
	/*!
	
		Main drawing buffer. Used to send data to the graphics card.
	
	*/
	struct Buffer
	{
		// // ----- CONSTRUCTORS ----- // //
	public:
		/*!
		 * @brief Constructor.
		 * @param target The GL Target (e.g. `GL_ARRAY_BUFFER`).
		 * @param size The size of the buffer in bytes
		 * @param binding The id to bind to. Usually leave as default.
		*/
		Buffer(std::uint32_t target, std::size_t size, std::uint32_t binding = 0);
		~Buffer();

		/*!
		 * @brief Use move constructor instead of copy constructor.
		*/
		Buffer(const Buffer&) = delete;

		/*!
		 * @brief Move constructor.
		*/
		Buffer(Buffer&&);

		/*!
		 * @brief Use move assign instead of copy assign.
		*/
		Buffer& operator = (const Buffer&) = delete;

		/*!
		 * @brief Move assign.
		*/
		Buffer& operator = (Buffer&&);


		// // ----- GENERAL METHODS ----- // //

		/*!
		 * @brief Call `glGenBuffers` to generate a new GL buffer.
		 * @param size The size in bytes of the buffer to generate.
		 * @return The new buffer ID. This will be 0 if the buffer 
		 * failed to generate.
		*/
		std::uint32_t generate_buffer(std::size_t size);

		/*!
		 * @brief Access the GL buffer and copy the contents to `dest`.
		*/
		void read_buffer(void* dest);

		/*!
		 * @brief Send data to the GL buffer.
		 * @param offset The number of bytes to skip before writing.
		 * @param size The size in bytes of the data to write.
		 * @param data A pointer to the actual data.
		*/
		void upload(std::size_t offset, std::size_t size, const void* data);

		/*!
		 * @brief Map the local stored data to the GL buffer.
		 * @param size The number of bytes to map.
		 * @param offset The number of bytes to skip before mapping the first byte.
		 * @return A pointer to the GL memory location. New data can then be copied 
		 * to the buffer.
		*/
		void* map_buffer(std::size_t size, std::size_t offset = 0u);

		/*!
		 * @brief Unmap the buffer.
		*/
		void unmap();

		/*!
		 * @brief Append new data to the end of the existing buffer. 
		 * @param size The number of bytes to append.
		 * @param data The data to append.
		 * @return The offset (in number of bytes) to the start of where 
		 * this data is stored.
		*/
		std::uint32_t append(std::size_t size, const void* data);

		/*!
		 * @brief Resize the buffer to `new_size` number of bytes.
		*/
		void resize(std::size_t new_size);

		/*!
		 * @brief Allocate `new_capacity` number of bytes to the buffer.
		*/
		void reserve(std::size_t new_capacity);

		/*!
		 * @brief Make this buffer 'active' in relation to OpenGL
		*/
		void bind() const;

		/*!
		 * @brief Make this buffer 'active' with `target` in relation to OpenGL
		 * @param target The OpenGL target (e.g. `GL_TEXTURE_2D`)
		*/
		void bind(std::uint32_t target);

		/*!
		 * @brief Set the GL binding ID that this buffer is associated with to `binding_id`
		*/
		void set_binding_id(std::uint32_t binding_id);

		// // INPUTTING WITH VECTORS // //

		/*!
		 * @brief Append a `std::vector` of `std::vector`s to the end of the existing buffer.
		 * @brief Append a `std::vector` of `std::vector`s to the end of the existing buffer.
		 * @param data The data to be sent to the buffer
		 * @return The offset (in number of bytes) to the start of where 
		 * this data is stored.
		*/
		template<typename T>
		auto append(const std::vector<std::vector<T>>& data)
		{
			std::size_t numElements = 0;
			for (auto & i : data) { numElements += i.size(); }
			return append(numElements * sizeof(T), data.data());
		}

		/*!
		 * @brief Append a `std::vector` to the end of the existing buffer.
		 * @param data The data to be sent to the buffer
		 * @return The offset (in number of bytes) to the start of where
		 * this data is stored.
		*/
		template<typename T>
		auto append(const std::vector<T>& data)
		{
			return append(data.size() * sizeof(T), data.data());
		}

		/*!
		 * @brief Send an `std::vector` to the GL buffer.
		 * @param offset The number of bytes to skip before writing.
		 * @param size The size in bytes of the data to write.
		 * @param data A pointer to the actual data.
		*/
		template<typename T>
		auto upload(const std::vector<T>& data)
		{
			return upload(0, data.size() * sizeof(T), data.data());
		}

		/*!
		 * @brief Send data to the GL buffer.
		 * @param offset The number of bytes to skip before writing.
		 * @param size The size in bytes of the data to write.
		 * @param data A pointer to the actual data.
		*/
		template<typename T>
		auto upload(T data)
		{
			return upload(0, sizeof(T), &data);
		}

		/*!
		 * @brief Map the local stored data to the GL buffer.
		 * @param size The number of bytes to map.
		 * @param offset The number of bytes to skip before mapping the first byte.
		 * @return A pointer to the GL memory location. New data can then be copied
		 * to the buffer.
		*/
		template<typename T>
		T* map_buffer(std::size_t size, std::size_t offset = 0u) {
			return (T*)map_buffer(size * sizeof(T), offset * sizeof(T));
		}


		// // GETTERS // //

		/*!
		 * @return The OpenGL buffer ID associated with this buffer.
		*/
		std::uint32_t buffer_id() const noexcept { return m_buffer_id; }
		
		/*!
		 * @return The OpenGL binding point ID associated with this buffer.
		*/
		std::uint32_t binding_id() const noexcept { return m_binding_id; }
		
		/*!
		 * @return The number of bytes that this buffer can hold.
		*/
		std::size_t capacity() const noexcept { return m_capacity; }
		
		/*!
		 * @return The number of bytes currently being used in the buffer.
		*/
		std::size_t size() const noexcept { return m_size; }

		// // ----- MEMBER VARIABLES ----- // //
	private:
		std::size_t		m_size;
		std::size_t		m_capacity;
		std::uint32_t	m_buffer_id;
		std::uint32_t	m_target;
		std::uint32_t	m_binding_id;
	};
}
#endif