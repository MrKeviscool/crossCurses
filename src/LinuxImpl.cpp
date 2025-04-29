#ifdef __linux__

#include "CrossCurses.hpp"

#include <iostream>
#include <curses.h>
#include <cstring>

#define THROW_CURSERR() {nocbreak(); endwin(); std::cerr << "failed in: " << __func__ << '\n'; throw std::exception();}


WINDOW* window = nullptr;

void initaliseConsole(){
    window = initscr();
    if(!window)
        THROW_CURSERR();
}

void setConsoleTitle(const char* title){
    //not implemented
}

void setCursorPos(const short x, const short y){
    if(move(y, x) == ERR)
        THROW_CURSERR();
}

void writeText(const char* string){
    int lenOfText = std::strlen(string);
    // if(lenOfText + window->_curx > window->_maxx)

    if(addstr(string) == ERR)
        THROW_CURSERR();
}

void writeChar(const char character){
    if(addch(character) == ERR)
        THROW_CURSERR();
}

void refreshScr(){
    if(refresh() == ERR)
        THROW_CURSERR();
}

void deInitaliseConsole(){
    if(endwin() == ERR)
        THROW_CURSERR();
}

#endif