
#pragma once

#include <SFML/Graphics.hpp>

struct Theme {
    ~Theme() = default;

    struct playerOne {
        auto inline static const pawn = sf::Color(0xD0, 0x00, 0x00);
        auto inline static const score = sf::Color(0xD0, 0x00, 0x00);
        auto inline static const closer = sf::Color(0xE0, 0x00, 0x00);
        auto inline static const further = sf::Color(0xFF, 0x85, 0x85);
    };

    struct playerTwo {
        auto inline static const pawn = sf::Color(0xFF, 0xBA, 0x08);
        auto inline static const score = sf::Color(0xFF, 0xBA, 0x08);
        auto inline static const closer = sf::Color(0xF5, 0xC4, 0x3D);
        auto inline static const further = sf::Color(0xB0, 0x8B, 0x1C);
    };

    struct board {
        auto inline static const background = sf::Color(0xF0, 0xF2, 0xEF);
        auto inline static const outOfGame = sf::Color(0xF0, 0xF2, 0xEF);
        auto inline static const empty = sf::Color(0x76, 0x82, 0x73);
        auto inline static const option = sf::Color(0x36, 0xE7, 0xE7);
        auto inline static const chosen = sf::Color(0x2C, 0x25, 0xF8);
        auto inline static const outline = sf::Color(0x8A, 0x96, 0x88);
        auto inline static const arial = sf::Font("ARIAL.TTF");
    };
};
