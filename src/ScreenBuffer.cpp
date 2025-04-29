#include "ScreenBuffer.hpp"

ScreenBuffer::ScreenBuffer(const short sizeX, const short sizeY) :
	sizeX(sizeX), sizeY(sizeY),
	charMatrix(sizeX, sizeY),
	attributeMatrix(sizeX, sizeY),
	changedMatrix(sizeX, sizeY)
{
	charMatrix.fill(' ');
	attributeMatrix.fill({false, false, TextAttribute::Color::Normal});
	changedMatrix.fill(false);
}

char ScreenBuffer::getChar(const short x, const short y) const {
	return charMatrix.at(x, y);
}

TextAttribute ScreenBuffer::getAttribute(const short x, const short y) const {
	return attributeMatrix.at(x, y);
}

void ScreenBuffer::setChar(const short x, const short y, char character) {
	charMatrix[x][y] = character;
	changedMatrix[x][y] = true;
}

void ScreenBuffer::setAttribute(const short x, const short y, const TextAttribute attribute) {
	attributeMatrix[x][y] = attribute;
	changedMatrix[x][y] = true;
}

bool ScreenBuffer::getChanged(const short x, const short y) const {
	return changedMatrix.at(x, y);
}

void ScreenBuffer::resetChangedList() {
	changedMatrix.fill(false);
}