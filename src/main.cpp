#include "CrossCurses.hpp"

#include <string>
#include <iostream>

int main() {
	const std::string outStr = "Hello World!";

	initaliseConsole();
	setConsoleTitle("testing");
	for (std::size_t i = 0; i < outStr.size(); i++) {
		setCursorPos(i, i);
		writeChar(outStr[i]);
	}
	refreshScr();


	std::cin.get();
}