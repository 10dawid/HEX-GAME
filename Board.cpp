#include <SFML/Graphics.hpp>

#include "Theme.h"
#include "Tile.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
auto createBaseMap() -> std::vector<std::vector<Tile>>;
auto createBaseHexLayout (std::vector<std::vector<Tile>>&) -> std::vector<std::vector<sf::CircleShape>>;
auto updateState(sf::CircleShape&, Tile&, State) -> void;
auto getScore( std::vector<std::vector<Tile>>&) -> std::pair<int, int>;
auto isLockedPawn(Position, std::vector<std::vector<Tile>>) -> bool;
auto isGameOver(std::pair<int, int>) -> bool;
auto findNewFileName() -> std::string;




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

    Board(const std::vector<std::vector<Tile>> &amap, const std::vector<std::vector<sf::CircleShape>> &hex_layout)
        : map(amap),
          hexLayout(hex_layout) {}

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


auto createBaseMap() -> std::vector<std::vector<Tile>> {

    auto map = std::vector<std::vector<Tile>>();

    for (int column = 0; column < 9; ++column) {
        auto columnVector = std::vector<Tile>();

        int row = 0;
        auto position = Position(column, row);
        while (position.isValid()) {
            columnVector.push_back(Tile(position, State::Empty));
            position.row += 1;
        }

        map.push_back(columnVector);
    }

    map[0][0].state = State::Player1;
    map[4][8].state = State::Player1;
    map[8][0].state = State::Player1;

    map[0][4].state = State::Player2;
    map[4][0].state = State::Player2;
    map[8][4].state = State::Player2;

    map[3][4].state = State::OutOfGame;
    map[4][3].state = State::OutOfGame;
    map[5][4].state = State::OutOfGame;

    return map;
}
auto createBaseHexLayout (std::vector<std::vector<Tile>>& map) -> std::vector<std::vector<sf::CircleShape>>{
    auto hexLayout = std::vector<std::vector<sf::CircleShape>>();
    auto startingPoint = 0;

    for (int column = 0; column < map.size(); ++column) {
        auto hexColumn = std::vector<sf::CircleShape>();
        if (column <= 4) {
            startingPoint = 250 - (column * 50);
        } else {
            startingPoint = 100 + ((column - 5) * 50);
        }

        // auto columnX = 400 + column * 100;
        auto columnX = 400 + column * 87;

        for (int row = 0; row < map[column].size(); ++row) {
            auto hexagon = sf::CircleShape(50,6);
            hexagon.setPosition({static_cast<float>(columnX), static_cast<float>(startingPoint)});
            switch (map[column][row].state){
                case State::Empty: hexagon.setFillColor(Theme::board::empty); break;
                case State::OutOfGame: hexagon.setFillColor(Theme::board::outOfGame); break;
                case State::Player1: hexagon.setFillColor(Theme::playerOne::pawn); break;
                case State::Player2: hexagon.setFillColor(Theme::playerTwo::pawn); break;
            }


            startingPoint += 100;
            hexagon.setOutlineColor(Theme::board::outline);
            hexagon.setOutlineThickness(7);
            hexagon.rotate(sf::degrees(90));
            hexColumn.push_back(hexagon);


        }
        hexLayout.push_back(hexColumn);
    }

    return hexLayout;
}
auto updateState(sf::CircleShape& hex, Tile& tile, State newState) -> void {
    tile.state = newState;
    switch (newState){
        case State::Empty: hex.setFillColor(Theme::board::empty); break;
        case State::OutOfGame: hex.setFillColor(Theme::board::outOfGame); break;
        case State::Player1: hex.setFillColor(Theme::playerOne::pawn); break;
        case State::Player2: hex.setFillColor(Theme::playerTwo::pawn); break;
    }
}
auto getScore( std::vector<std::vector<Tile>>& map ) -> std::pair<int, int> {
    auto [player1, player2] = std::pair<int, int>{0,0};
    for (auto column : map)
        for (auto tile : column) {
            switch (tile.state) {
                case State::Player1: player1++; break;
                case State::Player2: player2++; break;
                default: break;
            }
        }

    return {player1,player2};
}
auto getPlayerOneOptions(std::vector<std::vector<Tile>> map) -> std::vector<Tile> {
    auto options = std::vector<Tile>();
    for (const auto& column: map) {
        for (const auto& tile: column) {
            if (tile.state == State::Player1) options.push_back(tile);
        }
    }
    return options;

}
auto getPlayerTwoOptions(std::vector<std::vector<Tile>> map) -> std::vector<Tile> {
    auto options = std::vector<Tile>();
    for (const auto& column: map) {
        for (const auto& tile: column) {
            if (tile.state == State::Player2) options.push_back(tile);
        }
    }
    return options;
}
auto resetOutlines(std::vector<std::vector<sf::CircleShape>>& hexLayout) -> void {
    for (auto& column: hexLayout) {
        for (auto& hexagon: column) {
            hexagon.setOutlineColor(Theme::board::outline);
        }
    }
}
auto isLockedPawn(Position position, std::vector<std::vector<Tile>> map) -> bool {
    auto emptyPositions = std::vector<Position>();

    for (auto pos : findCloserPositions(position)) {
        if (map[pos.column][pos.row].state == State::Empty) emptyPositions.push_back(pos);
    }
    for (auto pos : findFurtherPositions(position)) {
        if (map[pos.column][pos.row].state == State::Empty) emptyPositions.push_back(pos);
    }

    return emptyPositions.empty();
}
auto isGameOver(Board board) -> bool {
    auto score = getScore(board.map);
    if (score.first == 0 || score.second == 0 || score.first + score.second == 58)
         return true;
    auto allP1PawnsLocked = true;

    for (auto pawn : getPlayerOneOptions(board.map))
        if (!isLockedPawn(pawn.position, board.map))
            allP1PawnsLocked = false;

    auto allP2PawnsLocked = true;
    for (auto pawn : getPlayerOneOptions(board.map))
        if (!isLockedPawn(pawn.position, board.map))
            allP2PawnsLocked = false;
    if (allP1PawnsLocked || allP2PawnsLocked)
        return true;
    return false;
}
auto makeMove(std::vector<std::vector<Tile>> map, Tile from, Tile to) -> std::vector<std::vector<Tile>>{
    auto oppositePlayer = (from.state == State::Player1 ? State::Player2 : State::Player1);
    if (from.position.isValid() && to.position.isValid()) {
        if (to.state == State::Empty) {
            auto closer = findCloserPositions(from.position);
            for (auto& closerPosition: from.closerPositions) {
                if (closerPosition.column == to.position.column && closerPosition.row == to.position.row) {
                    map[to.position.column][to.position.row].state = from.state;
                    for (auto target : findCloserPositions(to.position)) {
                        if (map[target.column][target.row].state == (from.state == State::Player1 ? State::Player2 : State::Player1)) {
                            map[target.column][target.row].state = map[from.position.column][from.position.row].state;
                        }
                    }
                    return map;
                }
            }
            for (auto& furtherPosition: from.furtherPositions) {
                if (furtherPosition.column == to.position.column && furtherPosition.row == to.position.row) { // jezeli pozycja to daleki skok
                    map[to.position.column][to.position.row].state = from.state; // przejecie pionka
                    map[from.position.column][from.position.row].state = State::Empty;
                    for (auto target : findCloserPositions(to.position)) {
                        if (map[target.column][target.row].state == oppositePlayer) {
                            map[target.column][target.row].state = map[from.position.column][from.position.row].state;
                        }
                    }
                    return map;
                }
            }
        }
    }
    return map;
}
auto saveBoardToTextFile(Board const& board, Turn turn, std::string filename) -> void {
    auto file = std::fstream(filename, std::ios::out | std::ios::trunc);

    auto turnInt = int();
    switch (turn) {
        case Turn::P1Choose: turnInt = 1; break;
        case Turn::P1Move: turnInt = 1; break;
        case Turn::P2Choose: turnInt = 2; break;
        case Turn::P2Move: turnInt = 2; break;
    }
    file << turnInt << std::endl;

    for (auto const& column : board.map) {
        for (auto const& tile : column) {
            auto value = 0;
            switch (tile.state) {
                case State::Player1: value = 1; break;
                case State::Player2: value = 2; break;
                case State::Empty: value = 3; break;
                case State::OutOfGame: value = 4; break;
            }
            file << value;
            if (tile.position.row < column.size() - 1)
                file << " ";
        }
        file << std::endl;
    }
}
auto loadBoardFromFile(std::string filename) -> std::pair<Board, Turn> {
    auto file = std::fstream(filename);
    auto board = Board();

    auto turn = Turn::P1Choose;
    auto line = std::string();
    auto lineAsStream = std::stringstream(line);
    auto singleString = std::string();

    if (std::getline(file, line)) {
        lineAsStream = std::stringstream(line);

        if (lineAsStream >> singleString) {
            switch (std::stoi(singleString)) {
                case 1 : turn = Turn::P1Choose; break;;
                case 2 : turn = Turn::P2Choose; break;
            }
        }
    }
    auto map = std::vector<std::vector<Tile>>();
    auto columnIndex = 0;
    auto column = std::vector<Tile>();
    while (std::getline(file, line)) {
        column.clear();
        lineAsStream = std::stringstream(line);
        auto rowIndex = 0;
        while (lineAsStream >> singleString) {
            State state;

            switch (std::stoi(singleString)) {
                case 1: state = State::Player1; break;
                case 2: state = State::Player2; break;
                case 3: state = State::Empty; break;
                case 4: state = State::OutOfGame; break;
                default: break;
            }

            column.push_back(Tile(Position(columnIndex, rowIndex), state));
            rowIndex ++;
            lineAsStream.clear();
        }
        map.push_back(column);
        columnIndex ++;
    }


    board.map = map;
    board.hexLayout = createBaseHexLayout(board.map);
    return {Board(map, createBaseHexLayout(map)), turn};
}
auto findNewFileName() -> std::string {

    auto base = std::string("/Users/dawidleczycki/CLionProjects/HEX-GAME/cmake-build-debug/savedGames/save");

    auto i = 1;
    auto candidate = (base + std::to_string(i) + ".txt");
    while (std::filesystem::exists(std::filesystem::path(candidate))) {
        i += 1;
        candidate = (base + std::to_string(i) + ".txt");
    }

    return candidate;

}

