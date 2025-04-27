#pragma once
#include "Matrix.hpp"

class ScreenBuffer {
public:

	ScreenBuffer() = default;
	ScreenBuffer(const short sizeX, const short sizeY);

	char getChar(const short x, const short y) const;
	short getAttribute(const short x, const short y) const;

	void setChar(const short x, const short y, char character);
	void setAttribute(const short x, const short y, const short attribute);

	bool getChanged(const short x, const short y) const;
	void resetChangedList();

private:

	short sizeX = 0, sizeY = 0;

	//bool initalised = false;
	Matrix<char> charMatrix;
	Matrix<short> attributeMatrix;
	Matrix<bool> changedMatrix;

};