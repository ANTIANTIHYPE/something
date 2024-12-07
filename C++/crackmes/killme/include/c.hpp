#pragma once

#include <string>

class Color
{
public:
    enum Code 
    {
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        PINK = 95,
        DEFAULT = 39,
    };

    inline std::string colorize(const std::string& text, Code code, bool bold = false)
    {
        std::string ansiCode = "\x1B[";
        if (bold) ansiCode += "1;";
        ansiCode += std::to_string(code) + "m";
        return ansiCode + text + "\x1B[0m";
    }
};
