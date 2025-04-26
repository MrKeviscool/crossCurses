#include <iostream>
#include <cstring>
#include <cstdint>

#include "CrossCurses.hpp"
#include "ScreenBuffer.hpp"

#ifdef _WIN32
namespace win {
#include <Windows.h>
};

#undef STD_OUTPUT_HANDLE
#define STD_OUTPUT_HANDLE 4294967285U
#undef STD_INPUT_HANDLE
#define STD_INPUT_HANDLE 4294967286U


#define THROW_WINERROR() {std::cerr << "error in \"" << __func__ << "\", error code: " << win::GetLastError() << '\n'; throw std::exception();}

static win::HANDLE stdOutHandle, stdInHandle;
static Vec2 cursorPos = { 0,0 }, screenSize = { 0,0 };
static ScreenBuffer* screenBuffer = nullptr;

void initaliseConsole(), setConsoleTitle(const char* title),
setCursorPos(const short x, const short y), writeText(const char* text),
writeText(const char* text, const short attrubutes), writeChar(const char letter),
writeText(const char* text, const short attrubutes), setTextAttrs(const short attrs),
refresh(), setTextAttrs(const short attr), writeCharToTerm(const char character),
setTermCursor(const int x, const int y);

std::vector<Event> pollEvents();

//////////////////PUBLIC///////////////////

void initaliseConsole() {
	//init handles to std out and in
	stdOutHandle = win::GetStdHandle(STD_OUTPUT_HANDLE);
	if (win::GetLastError())
		THROW_WINERROR();
	stdInHandle = win::GetStdHandle(STD_INPUT_HANDLE);
	if (win::GetLastError())
		THROW_WINERROR();

	//get size of console
	win::CONSOLE_SCREEN_BUFFER_INFO scrInfo;
	if (!win::GetConsoleScreenBufferInfo(stdOutHandle, &scrInfo))
		THROW_WINERROR();
	screenSize = { scrInfo.dwSize.X, scrInfo.dwSize.Y };

	screenBuffer = ScreenBuffer::createScrBuffInstance(screenSize.x, screenSize.x);

	if (!win::SetConsoleMode(
		stdInHandle,
		ENABLE_ECHO_INPUT |
		ENABLE_LINE_INPUT |
		ENABLE_MOUSE_INPUT |
		ENABLE_WINDOW_INPUT
	)) THROW_WINERROR();
}

void setConsoleTitle(const char* title) {
	if (!win::SetConsoleTitleA(title))
		THROW_WINERROR();
}

void setCursorPos(const short x, const short y) {
	if (x >= screenSize.x || y >= screenSize.x) {
		std::cerr << "error, size above max buffer in: " << __func__;
		throw std::exception();
	}
	if (!win::SetConsoleCursorPosition(stdOutHandle, { x, y }))
		THROW_WINERROR();
}

//takes a null-terminated string and puts it at the cursor position
void writeText(const char* text) {
	std::size_t amountToWrite =  std::strlen(text);
	if (amountToWrite + cursorPos.x > screenSize.x) amountToWrite = screenSize.x;
	for (unsigned long x = 0; x < amountToWrite; x++)
		screenBuffer->setChar(cursorPos.x + x, cursorPos.y, text[x]);
}

void writeText(const char* text, const short attrubutes) {
	unsigned long amountToWrite = std::strlen(text);
	if (amountToWrite + cursorPos.x > screenSize.x) amountToWrite = screenSize.x;
	for (unsigned long x = 0; x < amountToWrite; x++)
		screenBuffer->setAttrChar(cursorPos.x + x, cursorPos.y, { text[x], attrubutes });
}

void writeChar(const char letter) {
	if (cursorPos.x == screenSize.x - 1) return;
	screenBuffer->setChar(cursorPos.x, cursorPos.y, letter);
}

void writeChar(const char letter, const short attributes) {
	if (cursorPos.x == screenSize.x - 1) return;
	screenBuffer->setAttrChar(cursorPos.x, cursorPos.y, { letter, attributes });
}


void refresh() {

	for (short x = 0; x < screenSize.x; x++) {
		for (short y = 0; y < screenSize.y; y++) {

			if (!screenBuffer->getChanged(x, y)) continue;

			const AttrChar curAttrChar = screenBuffer->getAttrChar(x, y);

			if (x == 0 || curAttrChar.attributes != screenBuffer->getAttrChar(x - 1, y).attributes)
				setTextAttrs(curAttrChar.attributes);

			if (curAttrChar.character == '\b') {
				writeCharToTerm('\b');
			}

			writeCharToTerm(curAttrChar.character);

		}
	}

	screenBuffer->resetChangedMatrix();

}

std::vector<Event> pollEvents() {
	const win::DWORD maxInputs = 5UL;
	win::INPUT_RECORD inputRecords[maxInputs];

	win::DWORD numOfEventsRead = 0;

	if (!win::ReadConsoleInputA(
		stdInHandle,
		inputRecords,
		maxInputs,
		&numOfEventsRead
	)) THROW_WINERROR();

	std::vector<Event> out(numOfEventsRead);
	for (win::DWORD i = 0; i < numOfEventsRead; i++) {

		win::INPUT_RECORD& curRec = inputRecords[i];
		Event& curEvent = out[i];

		const int controlOffset = 96;

		if (curRec.EventType == KEY_EVENT) {
			curEvent.eventType = Event::EventType::key;

			curEvent.ctrl = curRec.Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED);
			curEvent.shift = curRec.Event.KeyEvent.dwControlKeyState & SHIFT_PRESSED;

			curEvent.keyEventData.keyDown = curRec.Event.KeyEvent.bKeyDown;
			curEvent.keyEventData.repeatCount = curRec.Event.KeyEvent.wRepeatCount;
			curEvent.keyEventData.keyCode = curRec.Event.KeyEvent.wVirtualKeyCode;
			const char gotChar = curRec.Event.KeyEvent.uChar.AsciiChar;
			curEvent.keyEventData.character = (curEvent.ctrl && gotChar ? gotChar + controlOffset : gotChar); //got char is 0 if its ctrl so we dont ajust for ctrl adding 96
			if (curEvent.keyEventData.character == VK_RETURN) curEvent.keyEventData.character = '\n';
			else if (curEvent.keyEventData.character == VK_BACK) {
				curEvent.keyEventData.character = 0;
				writeChar('\b');
			}
		}
		else if (curRec.EventType == MOUSE_EVENT) {
			curEvent.eventType = Event::EventType::mouse;

			const win::DWORD buttonState = curRec.Event.MouseEvent.dwButtonState;
			if (buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) curEvent.mouseEventData.mouseButtonDown = Event::MouseEventData::MouseButtonDown::Left;
			else if (buttonState == RIGHTMOST_BUTTON_PRESSED) curEvent.mouseEventData.mouseButtonDown = Event::MouseEventData::MouseButtonDown::Right;

			const win::COORD coord = curRec.Event.MouseEvent.dwMousePosition;
			curEvent.mouseEventData.mousePos = { coord.X, coord.Y };

			switch (curRec.Event.MouseEvent.dwEventFlags) {
			case DOUBLE_CLICK:
				curEvent.mouseEventData.mouseEventType = Event::MouseEventData::MouseEventType::DoubleClick;
				break;
			case MOUSE_MOVED:
				curEvent.mouseEventData.mouseEventType = Event::MouseEventData::MouseEventType::MouseMoved;
				break;
			case MOUSE_WHEELED:
				curEvent.mouseEventData.mouseEventType = Event::MouseEventData::MouseEventType::Scrolled;
				curEvent.mouseEventData.scrollForward = static_cast<bool>(curRec.Event.MouseEvent.dwButtonState & 0xFFFF0000L);
				break;
			default:
				curEvent.mouseEventData.mouseEventType = Event::MouseEventData::MouseEventType::Normal;
			}
		}
		else if (curRec.EventType == WINDOW_BUFFER_SIZE_EVENT) {
			curEvent.eventType = Event::EventType::scrResize;
			const win::COORD newSize = curRec.Event.WindowBufferSizeEvent.dwSize;
			curEvent.newScreenSize = { newSize.X, newSize.Y };
		}
		else {
			curEvent.eventType = Event::EventType::invalidEvent;
		}
	}

	return out;
}


///////////////////PRIVATE////////////////////
void writeCharToTerm(const char character) {
	const bool result = win::WriteConsoleA(
		stdOutHandle,
		&character,
		1,
		NULL,
		NULL
	);
	if (!result)
		THROW_WINERROR();
}

void setTextAttrs(const short attr) {
	if (!win::SetConsoleTextAttribute(stdOutHandle, attr))
		THROW_WINERROR();
}

void setTermCursor(const short x, const short y) {
	if (!win::SetConsoleCursorPosition(stdOutHandle, { x, y }))
		THROW_WINERROR();
}

#elif __linux__

#endif