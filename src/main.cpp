#include "CrossCurses.hpp"

#include <string>
#include <iostream>

int main() {
	const std::string outStr = "Hello World!";

	initaliseConsole();
	setConsoleTitle("testing");
	for (std::size_t i = 0; i < outStr.size(); i++) {
		static int colIndex = static_cast<int>(TextAttribute::Color::Red);
		setCursorPos(i, i);
		setAttr(TextAttribute{ false, false, static_cast<TextAttribute::Color>(colIndex), TextAttribute::Color::Normal});
		writeChar(outStr[i]);
		if(colIndex == static_cast<int>(TextAttribute::Color::Blue)) colIndex = static_cast<int>(TextAttribute::Color::Red)-1;
		colIndex++;
	}
	refreshScr();
	std::cin.get();
	deInitaliseConsole();
}