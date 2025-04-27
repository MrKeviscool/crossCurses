#pragma once

#include <cstddef>
#include <stdexcept>
#include <cstring>

template <typename T>
class Matrix {
public:
	Matrix(const std::size_t x, const std::size_t y);
	Matrix() = default;
	~Matrix();

	Matrix(const Matrix& other);
	Matrix(Matrix&& other);
	
	void operator=(const Matrix& other) noexcept;
	void operator=(Matrix&& other) noexcept;

	void fill(const T& fillData);
	T* operator[](const std::size_t index);
	const T& at(const std::size_t x, const std::size_t y) const;
	std::size_t sizeX() const;
	std::size_t sizeY() const;

	void resize(const std::size_t newSizeX, const std::size_t newSizeY);


private:
	T* m_data = nullptr;
	std::size_t m_sizeX = 0, m_sizeY = 0;
};


template <typename T>
Matrix<T>::Matrix(const std::size_t x, const std::size_t y) : m_sizeX(x), m_sizeY(y) {
	m_data = new T[x * y];
}

template <typename T>
Matrix<T>::~Matrix() {
	delete[] m_data;
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other) {
	m_sizeX = other.sizeX;
	m_sizeY = other.sizeY;
	m_data = new T[m_sizeX * m_sizeY];

	std::memcpy(m_data, other.m_data, sizeof(T) * m_sizeX * m_sizeY);
}

template <typename T>
Matrix<T>::Matrix(Matrix<T>&& other) {
	m_sizeX = other.m_sizeX;
	m_sizeY = other.m_sizeY;

	m_data = other.m_data;
	other.m_data = nullptr;
}

template <typename T>
void Matrix<T>::operator=(const Matrix<T>& other) noexcept {
	m_sizeX = other.sizeX;
	m_sizeY = other.sizeY;
	m_data = new T[m_sizeX * m_sizeY];

	std::memcpy(m_data, other.m_data, sizeof(T) * m_sizeX * m_sizeY);
}

template <typename T>
void Matrix<T>::operator=(Matrix<T>&& other) noexcept {
	m_sizeX = other.m_sizeX;
	m_sizeY = other.m_sizeY;

	m_data = other.m_data;
	other.m_data = nullptr;
}

template <typename T>
void Matrix<T>::fill(const T& fillData) {
	for (std::size_t i = 0; i < m_sizeX * m_sizeY; i++) 
		m_data[i] = fillData;
}

template <typename T>
const T& Matrix<T>::at(const std::size_t x, const std::size_t y) const {
	if (x >= m_sizeX || y >= m_sizeY) throw std::out_of_range("matrix index out of range");
	return(m_data[x * m_sizeY + y]);
}

template <typename T>
std::size_t Matrix<T>::sizeX() const {
	return m_sizeX;
}

template <typename T>
std::size_t Matrix<T>::sizeY() const {
	return m_sizeY;
}

template <typename T>
T* Matrix<T>::operator[](const std::size_t index) {
	return(&m_data[index * m_sizeY]);
}

template <typename T>
void Matrix<T>::resize(const std::size_t newSizeX, const std::size_t newSizeY) {
	delete[] m_data;
	m_data = new T[newSizeX * newSizeY];
}