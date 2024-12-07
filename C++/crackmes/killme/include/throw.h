#pragma once

#include <stdexcept>
#include <random>

class Error : public std::runtime_error
{
public:
    Error(const std::string& message) : std::runtime_error(message) {}

    static std::string gen() noexcept
    {
        static std::random_device rd;
        static std::mt19937 mt(rd());
        static std::uniform_int_distribution<int> dist(0, 9);

        std::string messages[] = {
            "The abyss gazes back at you",
            "The void whispers secrets to the compiler",
            "The stars align in a pattern of despair",
            "The fabric of reality unravels",
            "The code succumbs to the weight of its own complexity",
            "The CPU weeps tears of silicon",
            "The memory leaks into the abyss",
            "The program drowns in a sea of uncertainty",
            "The logic gates tremble with fear",
            "The algorithmic vortex consumes all"
        };

        return messages[dist(mt)];
    }
};

#define THROW(x) throw Error(Error::gen())
