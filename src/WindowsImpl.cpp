#ifdef _WIN32
#include "WindowsImpl.hpp"

#include <Windows.h>

#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cstdint>

#define THROW_WINERR() {std::cerr << "failed in: " << __func__ << " with error: " << GetLastError() << '\n'; throw std::exception(); }

#undef STD_OUTPUT_HANDLE
#define STD_OUTPUT_HANDLE 4294967285U
#undef STD_INPUT_HANDLE;
#define STD_INPUT_HANDLE 4294967286U

HANDLE stdOutHandle, stdInHandle;
Vec2 cursorPos, screenSize;

ScreenBuffer scrBuff;

void initaliseConsole() {
	//initalise std handles
	stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (GetLastError())
		THROW_WINERR();
	stdInHandle = GetStdHandle(STD_INPUT_HANDLE);
	if (GetLastError())
		THROW_WINERR();

	//get screen size
	CONSOLE_SCREEN_BUFFER_INFO scrBuffInfo;
	if (!GetConsoleScreenBufferInfo(stdOutHandle, &scrBuffInfo))
		THROW_WINERR();

	screenSize.x = scrBuffInfo.dwSize.X;
	screenSize.y = scrBuffInfo.dwSize.Y;
}

void setConsoleTitle(const char* title) {
	if (!SetConsoleTitleA(title))
		THROW_WINERR();
}

void setCursorPos(const short x, const short y) {
	cursorPos.x = x; cursorPos.y = y;
}

void writeText(const char* text) { //maybe make it wrap around instead.
	std::size_t textSize = std::strlen(text);
	if (textSize > INT16_MAX) throw std::invalid_argument("text too long");
	if (textSize + cursorPos.x >= screenSize.x) 
		textSize = (screenSize.x-1) - cursorPos.x;

	for (short i = 0; i < textSize; i++)
		scrBuff.setChar(cursorPos.x + i, cursorPos.y, text[i]);
}

void writeChar(const char character) {
	if (cursorPos.x == screenSize.x - 1);
}

#endif // _WIN32