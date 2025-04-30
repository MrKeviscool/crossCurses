#include "ColorPair.hpp"


ColorPair::ColorPair(const TextAttribute::Color fg, const TextAttribute::Color bg, const  uint8_t index):
    fg(fg), bg(bg), index(index){}

// bool ColorPair
bool ColorPair::operator==(const ColorPair& other) const{
    return fg == other.fg && bg == other.bg;
}