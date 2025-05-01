#ifdef __linux__

#include "CrossCurses.hpp"
#include "ColorPair.hpp"
#include "Matrix.hpp"

#include <curses.h>

#include <iostream>
#include <cstring>
#include <vector>
#include <unordered_set>


#define THROW_CURSERR() {nocbreak(); endwin(); std::cerr << "failed in: " << __func__ << '\n'; throw std::exception();}

WINDOW* window = nullptr;

Matrix<ColIndexAttr> screenAttrs;


Vec2 getCursorPos(){
    Vec2 out;
    getyx(window, out.y, out.x);
    return out;
}

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

    const Vec2 scrSize {getmaxx(window), getmaxy(window)};
    screenAttrs.resize(scrSize.x, scrSize.y);
    screenAttrs.fill({0, 0});
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
    const Vec2 cursorPos = getCursorPos();
    const ColIndexAttr attrs = screenAttrs.at(cursorPos.x, cursorPos.y);

    attron(COLOR_PAIR(attrs.colIndex));
    if(addch(static_cast<chtype>(character) | attrs.attr) == ERR)
        THROW_CURSERR();
    attroff(COLOR_PAIR(attrs.colIndex));
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

    ColIndexAttr out;

    out.attr = (attribute.highlighted? highlighted : 0) |
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
        out.colIndex = ajustedUsedColPairSize; //set it to the active pair
    }
    else
        out.colIndex = foundIndex->index; //else set the found color pair index as the index

    const Vec2 curCursorPos = getCursorPos();
    screenAttrs[curCursorPos.x][curCursorPos.y] = out;

}

void setAttr(const TextAttribute attribute, short distanceToSet){
    const Vec2 screenSize {getmaxx(window), getmaxy(window)};
    const Vec2 originalCursorPos = getCursorPos();
    if(originalCursorPos.x + distanceToSet >= screenSize.x) distanceToSet = screenSize.x - originalCursorPos.x;


    Vec2 curCursorPos = originalCursorPos;
    for(short i = 0; i < distanceToSet; i++){
        setAttr(attribute);
        curCursorPos.x++;
        setCursorPos(curCursorPos.x, curCursorPos.y);
    }
    
}

#endif