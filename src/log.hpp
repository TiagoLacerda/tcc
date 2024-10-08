#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

#ifndef SSTREAM
#define SSTREAM
#include <sstream>
#endif

enum ANSIColor
{
    black = 30,
    red = 31,
    green = 32,
    yellow = 33,
    blue = 34,
    magenta = 35,
    cyan = 36,
    white = 37,
    brightBlack = 90,
    brightRed = 91,
    brightGreen = 92,
    brightYellow = 93,
    brightBlue = 94,
    brightMagenta = 95,
    brightCyan = 96,
    brightWhite = 97
};

// TODO: Document
template <typename T>
void log(const T &object, ANSIColor color = blue)
{
    std::stringstream stream;
    // SET ATTRIBUTES
    stream << "\x1b[";
    stream << color;
    // LOG OBJECT
    stream << "m";
    stream << object;
    // CLEAR ATTRIBUTES
    stream << "\x1B[0m";
    std::cout << stream.str() << std::endl;
}