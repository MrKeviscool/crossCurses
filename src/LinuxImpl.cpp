#ifdef __linux__

#include "CrossCurses.hpp"
#include "ColorPair.hpp"

#include <curses.h>

#include <iostream>
#include <cstring>
#include <vector>
#include <unordered_set>


#define THROW_CURSERR() {nocbreak(); endwin(); std::cerr << "failed in: " << __func__ << '\n'; throw std::exception();}

WINDOW* window = nullptr;

attr_t attributesToAdd = 0;
uint8_t activePair = 0;

void initaliseConsole(){
    window = initscr();
    if(!window)
        THROW_CURSERR();
    if(!has_colors()){
        std::cerr << "TERM DOES NOT HAVE COLOR SUPPORT\n";
        endwin();
        throw std::exception();
    }
    if(start_color())
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
    const auto strSize = std::strlen(string);
    for(std::size_t i = 0; i < strSize; i++)
        writeChar(string[i]);
}

void writeChar(const char character){
    const auto curActivePair = activePair;
    attron(COLOR_PAIR(activePair));
    if(addch(static_cast<chtype>(character) | attributesToAdd) == ERR)
        THROW_CURSERR();
    attroff(COLOR_PAIR(activePair));
}

void refreshScr(){
    if(refresh() == ERR)
        THROW_CURSERR();
}

void deInitaliseConsole(){
    if(endwin() == ERR)
        THROW_CURSERR();
}

void setAttr(const TextAttribute attribute){
    constexpr const short forgroundColors[] = {COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_BLUE};
    const short backgroundNormal = COLOR_BLACK;
	const attr_t highlighted = A_STANDOUT;
	const attr_t underlined  = A_UNDERLINE;

    static std::unordered_set<ColorPair> usedColorPairs;

    attributesToAdd = (attribute.highlighted? highlighted : 0) |
     (attribute.underlined? underlined : 0);

    
    const ColorPair gennedPair = { //create a temp pair based on inputs
        attribute.forgroundColor,
        attribute.backgroundColor,
        0
    };
    
    const auto foundIndex = usedColorPairs.find(gennedPair); //check if it exists


    if(foundIndex == usedColorPairs.end()){ //if it doseent exist, add it

        const uint8_t ajustedUsedColPairSize = usedColorPairs.size() + 1; //becuz i think 0 is invalid pair

        init_pair(ajustedUsedColPairSize, forgroundColors[static_cast<uint8_t>(attribute.forgroundColor)], //ignore this
        (attribute.backgroundColor != TextAttribute::Color::Normal? forgroundColors[static_cast<uint8_t>(attribute.backgroundColor)] : backgroundNormal));

        usedColorPairs.emplace(gennedPair.fg, gennedPair.bg, ajustedUsedColPairSize); //add it to the back of the set
        activePair = ajustedUsedColPairSize; //set it to the active pair
    }
    else
        activePair = foundIndex->index; //else set the found color pair index as the index

}

void setAttr(const TextAttribute attribute, short distanceToSet){
    setAttr(attribute);
}

#endif