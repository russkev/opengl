#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

struct Buffer
{
	// // BIG 6
	Buffer(std::uint32_t target, std::size_t size);
	~Buffer();
	Buffer(const Buffer&) = delete;
	Buffer(Buffer&&);
	Buffer& operator = (const Buffer&) = delete;
	Buffer& operator = (Buffer&&);

	// // GENERAL FUNCTIONS
	std::uint32_t GenerateBuffer(std::size_t size);
	template<typename T>
	void ReadBuffer(T& dest);
	void Upload(std::size_t, std::size_t, void*);
	void* Map(std::size_t size, std::size_t offset = 0u);
	void Unmap();
	std::uint32_t Append(std::size_t size, void* data);
	void Resize(std::size_t newSize);
	void Reserve(std::size_t newCapacity); //!!! Is this useful?
	void Bind() const;
	void Bind(std::uint32_t target);

	// // INPUTTING WITH VECTORS // //
	template<typename T>
	auto Append(std::vector<T> data) { 
		return Append(data.size() * sizeof(T), data.data());
	}
	template<typename T>
	auto Upload(std::vector<T> data) {
		return Upload(data.size() * sizeof(T), data.data());
	}

	template<typename T> 
	T* Map(std::size_t size, std::size_t offset = 0u) {
		return (T*)Map(size * sizeof(T), offset * sizeof(T));
	}

	// // GETTERS // //
	std::uint32_t bufferID() const noexcept { return m_bufferID; }
	std::size_t   capacity() const noexcept { return m_capacity; }
	std::size_t       size() const noexcept { return m_size; }

private:
	// // MEMBER VARIABLES // //
	std::size_t		m_size;
	std::size_t		m_capacity;
	std::uint32_t	m_bufferID;
	std::uint32_t	m_target;
};