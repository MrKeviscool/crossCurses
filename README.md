# Intro
crossCurses is a cross platform terminal graphics/control library that is loosely based on ncurses.

# Dependencies
building for windows will depend on the windows library (<Windows.h>)
building for *Nix will depend on ncurses.

# Building
there will be a CMake put here later aswell as releases. no need for main.cpp, that is a test file

# Usage
you must initialise the console with `initaliseConsole()` before any other command
you can then set the consoles title with `setConsoleTitle` if on windows

using `setCursorPos` will set the cursor positon for any following text writes.
text can be written with `writeText` or a single character can be written with `writeChar`

calling `refreshScr()` will write the text buffer the console. it only writes what was changed.

you can set text attributes such as color or boldness with `setAttr` and passing in a `textAttribute` type.
the `textAttribute` struct contains two boolians, `isHighligted` and `isUnderlined`, self explainitory.
it also contains an enum `Color` that specifies the structs `fg` and `bg` fields.

make sure to end your program with `deInitaliseConsole()` to avoid permanent (untill console close and re-open) effects.

enjoy :)

# Bugs

idk there is probably heaps. For instance, the windows and linux versions do not do the same thing when `setAttr` is called. windows sets the attribute for the next char, linux sets it for an infinite amount of chars untill its called again with diffrent colors.
just put any other bugs in issues i guess and ill have a look
