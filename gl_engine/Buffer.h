#ifndef GL_ENGINE_BUFFER_H
#define GL_ENGINE_BUFFER_H

#include <cstddef>
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

namespace gl_engine
{
	/*
	
		Main drawing buffer
	
	*/
	struct Buffer
	{
		// // ----- MEMBER VARIABLES ----- // //
	private:
		std::size_t		m_size;
		std::size_t		m_capacity;
		std::uint32_t	m_buffer_id;
		std::uint32_t	m_target;
		std::uint32_t	m_binding_id;

		// // ----- CONSTRUCTORS ----- // //
	public:
		Buffer(std::uint32_t target, std::size_t size, std::uint32_t binding = 0);
		~Buffer();
		Buffer(const Buffer&) = delete;
		Buffer(Buffer&&);
		Buffer& operator = (const Buffer&) = delete;
		Buffer& operator = (Buffer&&);

		// // ----- GENERAL METHODS ----- // //
		std::uint32_t generate_buffer(std::size_t size);
		void read_buffer(void* dest);
		void upload(std::size_t offset, std::size_t size, void* data);
		void* map_buffer(std::size_t size, std::size_t offset = 0u);
		void unmap();
		std::uint32_t append(std::size_t size, void* data);
		void resize(std::size_t new_size);
		void reserve(std::size_t new_capacity); //!!! Is this useful?
		void bind() const;
		void bind(std::uint32_t target);
		void set_binding_id(std::uint32_t binding_id);

		// // INPUTTING WITH VECTORS // //
		template<typename T>
		auto append(std::vector<std::vector<T>> data)
		{
			std::size_t numElements = 0;
			for (auto & i : data) { numElements += i.size(); }
			return append(numElements * sizeof(T), data.data());
		}
		template<typename T>
		auto append(std::vector<T> data)
		{
			return append(data.size() * sizeof(T), data.data());
		}
		template<typename T>
		auto upload(std::vector<T> data)
		{
			return upload(0, data.size() * sizeof(T), data.data());
		}
		template<typename T>
		auto upload(T data)
		{
			return upload(0, sizeof(T), &data);
		}

		template<typename T>
		T* map_buffer(std::size_t size, std::size_t offset = 0u) {
			return (T*)map_buffer(size * sizeof(T), offset * sizeof(T));
		}

		// // GETTERS // //
		std::uint32_t buffer_id() const noexcept { return m_buffer_id; }
		std::uint32_t binding_id() const noexcept { return m_binding_id; }
		std::size_t capacity() const noexcept { return m_capacity; }
		std::size_t size() const noexcept { return m_size; }
	};
} // namespace gl_engine
#endif