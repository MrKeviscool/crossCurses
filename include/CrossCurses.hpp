#pragma once

#include <vector>

struct Event;

void initaliseConsole();
void setConsoleTitle(const char* title);
void setCursorPos(const short x, const short y);
void writeText(const char* string);
void writeChar(const char character);
void refreshScr();
std::vector<Event> pollEvents();

struct Vec2 {
	short x, y;
};

struct Event {

	enum class EventType : uint8_t{
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
		enum class MouseEventType : uint8_t {
			Normal,
			DoubleClick,
			Scrolled,
			MouseMoved
		};
		enum class MouseButtonDown : uint8_t {
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

struct ConsoleMode {
	enum class ProccessMode : uint8_t {
		Cooked,
		Rare,
		Raw
	};

	bool echo = true;
	ProccessMode processMode;
};