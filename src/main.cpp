#include "CrossCurses.hpp"

#include <string>
#include <iostream>

int main() {
	const std::string outStr = "Hello World!";

	initaliseConsole();
	setConsoleTitle("testing");
	for (std::size_t i = 0; i < outStr.size(); i++) {
		static bool red = true;
		setCursorPos(i, i);
		setAttr(TextAttribute{ false, false, (red? TextAttribute::Color::Red : TextAttribute::Color::Green) , TextAttribute::Color::Normal});
		red = !red;
		writeChar(outStr[i]);
	}
	refreshScr();


	std::cin.get();
}