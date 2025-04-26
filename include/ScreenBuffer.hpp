#include <stdexcept>

#include <Matrix.hpp>

struct AttrChar {
	char character;
	short attributes;
};

class ScreenBuffer {
public:
	static ScreenBuffer& getScrBuffInstance();
	static ScreenBuffer& createScrBuffInstance(const short width, const short height);

	void resetChangedMatrix();
	short getSizeX() const;
	short getSizeY() const;

	AttrChar getAttrChar(const short x, const short y) const;
	bool getChanged(const short x, const short y) const;

	void setChar(const short x, const short y, const char character);
	void setAttr(const short x, const short y, const short attr);
	void setAttrChar(const short x, const short y, const AttrChar attrChar);

private:
	ScreenBuffer(const short width, const short height);
	ScreenBuffer() = default;
	void operator=(const ScreenBuffer&) const noexcept {};
	void operator=(ScreenBuffer&&) const noexcept {};

	static ScreenBuffer screenbuffer;
	static bool initalised;

	Matrix<AttrChar> m_characters;
	Matrix<bool>  m_changedThisFrame;

	short m_width = 0, m_height = 0;

};