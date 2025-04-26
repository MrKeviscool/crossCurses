#include <iostream>
#include <vector>

#include "Matrix.hpp"
#include "ScreenBuffer.hpp"
#include "CrossCurses.hpp"

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