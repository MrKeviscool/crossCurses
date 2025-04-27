#include "CrossCurses.hpp"

#include <string>
#include <iostream>

int main() {
	const std::string outStr = "Hello World!";

	initaliseConsole();
	setConsoleTitle("testing");
	for (int i = 0; i < outStr.size(); i++) {
		setCursorPos(i, i);
		writeChar(outStr[i]);
	}
	refresh();


	std::cin.get();
}