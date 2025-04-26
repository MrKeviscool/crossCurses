#include "ScreenBuffer.hpp"

bool ScreenBuffer::initalised = false; ScreenBuffer ScreenBuffer::screenbuffer;

ScreenBuffer* ScreenBuffer::createScrBuffInstance(const short width, const short height) {
	if (width < 0 || height < 0) throw std::length_error("cannot have negative size");

	initalised = true;

	screenbuffer.m_width = width;
	screenbuffer.m_height = height;
	screenbuffer.m_characters.resize(width, height);
	screenbuffer.m_changedThisFrame.resize(width, height);

	screenbuffer.m_characters.fill(AttrChar{ ' ', 0 });
	screenbuffer.resetChangedMatrix();

	return &screenbuffer;
}

ScreenBuffer* ScreenBuffer::getScrBuffInstance() {
	if (!initalised) throw std::logic_error("error. un-initialised ScreenBuffer");
	return &screenbuffer;
}

void ScreenBuffer::resetChangedMatrix() {
	m_changedThisFrame.fill(false);
}

short ScreenBuffer::getSizeX() const {
	return m_width;
}

short ScreenBuffer::getSizeY() const {
	return m_height;
}

AttrChar ScreenBuffer::getAttrChar(const short x, const short y) const {
	return m_characters.at(x, y);
}

bool ScreenBuffer::getChanged(const short x, const short y) const {
	return m_changedThisFrame.at(x, y);
}

void ScreenBuffer::setChar(const short x, const short y, const char character){
	m_characters[x][y].character = character;
	m_changedThisFrame[x][y] = true;
}

void ScreenBuffer::setAttr(const short x, const short y, const short attr) {
	m_characters[x][y].attributes = attr;
	m_changedThisFrame[x][y] = true;
}

void ScreenBuffer::setAttrChar(const short x, const short y, const AttrChar attrChar) {
	m_characters[x][y] = attrChar;
	m_changedThisFrame[x][y] = true;
}