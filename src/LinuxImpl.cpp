#ifdef __linux__

#include "CrossCurses.hpp"

#include <iostream>
#include <curses.h>


#define THROW_CURSERR() {std::cerr << "failed in: " << __func__ << '\n'; throw std::exception();}


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
    if(move(x, y) == ERR)
        THROW_CURSERR();
}

void writeText(const char* string){
    if(addstr(string) == ERR)
        THROW_CURSERR();
}

void writeChar(const char character){
    if(addch(character) == ERR)
        THROW_CURSERR();
}

void refreshScr(){
    refresh();
}


#endif