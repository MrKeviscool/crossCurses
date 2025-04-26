#include <iostream>
#include <vector>

#include "Matrix.hpp"

struct Event;

void initaliseConsole();
void setConsoleTitle(const char* title);
void setCursorPos(const short x, const short y);
void writeText(const char* text, const unsigned long amountToWrite);
std::vector<Event> pollEvents();
void writeChar(const char letter);
void refresh();

#ifdef __linux__
void writeText(const char* text); 
#endif

#ifndef DEFINED_VEC2
struct Vec2 {
	short x, y;
};
#define DEFINED_VEC2
#endif

struct Event {

	enum class EventType {
		key,
		mouse,
		scrResize,
		invalidEvent,
	};
	
	EventType eventType;
	bool ctrl, shift;

	struct KeyEventData {
		bool keyDown;
		short repeatCount;
		short keyCode;
		char character;
	};

	struct MouseEventData {
		enum class MouseEventType {
			Normal,
			DoubleClick,
			Scrolled,
			MouseMoved
		};
		enum class MouseButtonDown {
			None,
			Left,
			Middle,
			Right,
		};
		Vec2 mousePos;
		MouseEventType mouseEventType;
		MouseButtonDown mouseButtonDown;
		bool scrollForward;
	};

	union {
		KeyEventData keyEventData;
		MouseEventData mouseEventData;
		Vec2 newScreenSize;
	};

};


#ifdef _WIN32

namespace win {
	#include <Windows.h>
};

#undef STD_OUTPUT_HANDLE
#define STD_OUTPUT_HANDLE 4294967285U
#undef STD_INPUT_HANDLE
#define STD_INPUT_HANDLE 4294967286U

#define THROW_WINERROR() {std::cerr << "error in \"" << __func__ << "\", error code: " << win::GetLastError() << '\n'; throw std::exception();}

win::HANDLE stdOutHandle, stdInHandle;
win::CONSOLE_SCREEN_BUFFER_INFO scrBuffInfo;

void initaliseConsole() {
	stdOutHandle = win::GetStdHandle(STD_OUTPUT_HANDLE);
	if (win::GetLastError())
		THROW_WINERROR();
	stdInHandle = win::GetStdHandle(STD_INPUT_HANDLE);
	if (win::GetLastError())
		THROW_WINERROR();

	if (!win::GetConsoleScreenBufferInfo(stdOutHandle, &scrBuffInfo))
		THROW_WINERROR();
	if (!win::SetConsoleMode(
		stdInHandle,
		ENABLE_ECHO_INPUT  |
		ENABLE_LINE_INPUT  |
		ENABLE_MOUSE_INPUT |
		ENABLE_WINDOW_INPUT
	)) THROW_WINERROR();
}

void setConsoleTitle(const char* title) {
	if (!win::SetConsoleTitleA(title)) 
		THROW_WINERROR();
}

void setCursorPos(const short x, const short y) {
	if (x >= scrBuffInfo.dwSize.X || y >= scrBuffInfo.dwSize.Y) {
		std::cerr << "error, size above max buffer in: " << __func__;
		throw std::exception();
	}
	if (!win::SetConsoleCursorPosition(stdOutHandle, { x, y })) 
		THROW_WINERROR();
}

void writeText(const char* text, const unsigned long amountToWrite) {
	const bool result = win::WriteConsoleA(
		stdOutHandle,
		text,
		amountToWrite,
		NULL,
		NULL
	);
	if (!result)
		THROW_WINERROR();
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
				writeText("\b \b", 3);
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

void writeChar(const char letter) {
	const bool result = win::WriteConsoleA(
		stdOutHandle,
		&letter,
		1,
		NULL,
		NULL
	);
	if (!result)
		THROW_WINERROR();
}

void refresh() {

}

#elif __linux__

#endif

int main() {
	initaliseConsole();
	setConsoleTitle("Curse");
	for (;;) {
		const std::vector<Event> events = pollEvents();
		for (const auto& event : events) {
			if (event.eventType == Event::EventType::key) {
				if (!event.keyEventData.character || !event.keyEventData.keyDown) continue;
				for (short i = 0; i < event.keyEventData.repeatCount; i++) std::cout << event.keyEventData.character;
				//std::cout << "int key: " << (int)event.keyEventData.character << " keycode: " << event.keyEventData.keyCode << '\n';
				if (event.keyEventData.character == 'c' && event.ctrl) return 0;
			}
			else if (event.eventType == Event::EventType::mouse) {
				std::cout << "MOUSE SMTH SMTH!!!\n";
			}
			else if (event.eventType == Event::EventType::scrResize) {
				std::cout << "new window size: " << event.newScreenSize.x << ", " << event.newScreenSize.y << '\n';
			}
		}
	}
	std::cin.get();
	return 0;
}