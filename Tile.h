#pragma once

#include <vector>

struct Position {
    ~Position() = default;

    int column;
    int row;

    Position(int column, int row)
    : column(column),
      row(row) {
    }

    auto isValid() -> bool;

};
enum class State {
    Player1, Player2, Empty, OutOfGame
};
enum class Turn {
    P1Choose, P2Choose, P1Move, P2Move
};
auto findCloserPositions(Position position) -> std::vector<Position>;
auto findFurtherPositions(Position position) -> std::vector<Position>;
struct Tile {
    ~Tile() = default;
    Tile() = default;

    Position position;
    State state;
    std::vector<Position> closerPositions;
    std::vector<Position> furtherPositions;


    Tile(const Position &position, State state)
        : position(position),
          state(state),
          closerPositions(findCloserPositions(position)),
          furtherPositions(findFurtherPositions(position)) {
    }
};