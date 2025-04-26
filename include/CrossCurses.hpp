#pragma once
#include <vector>

struct Event;

void initaliseConsole();
void setConsoleTitle(const char* title);
void setCursorPos(const short x, const short y);
void writeText(const char* text, const short amountToWrite);
void writeChar(const char letter);
void refresh();
std::vector<Event> pollEvents();

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
