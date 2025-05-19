#pragma once
#include "Tile.h"
#include <SFML/Graphics.hpp>

auto createBaseMap() -> std::vector<std::vector<Tile>>;
auto createBaseHexLayout (std::vector<std::vector<Tile>>&) -> std::vector<std::vector<sf::CircleShape>>;
auto updateState(sf::CircleShape&, Tile&, State) -> void;
auto getScore( std::vector<std::vector<Tile>>&) -> std::pair<int, int>;
auto getPlayerOneOptions(std::vector<std::vector<Tile>>) -> std::vector<Tile>;
auto getPlayerTwoOptions(std::vector<std::vector<Tile>>) -> std::vector<Tile>;
auto resetOutlines(std::vector<std::vector<sf::CircleShape>>&) -> void;
auto isLockedPawn(Position, std::vector<std::vector<Tile>>) -> bool;
auto makeMove(std::vector<std::vector<Tile>>, Tile, Tile) -> std::vector<std::vector<Tile>>;



struct Board {
    std::vector<std::vector<Tile>> map;
    std::vector<std::vector<sf::CircleShape>> hexLayout;

    Board() = default;

    Board(const Board &other)
        : map(other.map),
          hexLayout(other.hexLayout) {
    }

    Board(Board &&other) noexcept
        : map(std::move(other.map)),
          hexLayout(std::move(other.hexLayout)) {
    }
    Board& operator=(const Board &other) {
        if (this == &other) {
            return *this;
        }
        map = other.map;
        hexLayout = other.hexLayout;
        return *this;
    }

    Board(const std::vector<std::vector<Tile>> &map, const std::vector<std::vector<sf::CircleShape>> &hex_layout)
        : map(map),
          hexLayout(hex_layout) {
    }
};

struct Save {
    sf::Text text;
    sf::RectangleShape button;
    std::string fileName;


    Save(const sf::Text &text, const sf::RectangleShape &button, const std::string &file_name)
        : text(text),
          button(button),
          fileName(file_name) {
    }
};

auto isGameOver(Board board) -> bool;
auto loadBoardFromFile(std::string filename) -> std::pair<Board, Turn>;
auto saveBoardToTextFile(Board const& board, Turn turn, std::string filename) -> void;
auto findNewFileName() -> std::string;
