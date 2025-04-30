#include "CrossCurses.hpp"

struct ColorPair{
    TextAttribute::Color fg, bg;
    uint8_t index;

    ColorPair(const TextAttribute::Color fg, const TextAttribute::Color bg, const  uint8_t index);

    bool operator==(const ColorPair& other) const;
};

//custom std hash for ColorPair
template<>
struct std::hash<ColorPair>{
    std::size_t operator()(const ColorPair& colPair) const {
        return std::hash<uint16_t>{}((static_cast<uint16_t>(colPair.fg)  << 8) | static_cast<uint16_t> (colPair.bg));
    }
};