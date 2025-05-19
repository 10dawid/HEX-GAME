#include <vector>

struct Position {
    int column;
    int row;

    Position(int column, int row)
        : column(column),
          row(row) {
    }

    auto isValid()  ->  bool ;
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

    Tile() = default;
};

auto Position::isValid()  ->   bool {
    switch (column) {
        case 0: return (row >= 0 && row <= column + 4);
        case 1: return (row >= 0 && row <= column + 4);
        case 2: return (row >= 0 && row <= column + 4);
        case 3: return (row >= 0 && row <= column + 4);
        case 4: return (row >= 0 && row <= column + 4);
        case 5: return (row >= 0 && row <= 7);
        case 6: return (row >= 0 && row <= 6);
        case 7: return (row >= 0 && row <= 5);
        case 8: return (row >= 0 && row <= 4);
        default: return false;
    }
}

auto findCloserPositions(Position position) -> std::vector<Position> {
    if (position.isValid()) {
        auto closerPositions = std::vector<Position>();
        auto suspects = std::vector<Position>();

        suspects.push_back({position.column, position.row - 1});
        suspects.push_back({position.column, position.row + 1});

        if (position.column == 4) {
            suspects.push_back({position.column - 1, position.row});
            suspects.push_back({position.column - 1, position.row - 1});
            suspects.push_back({position.column + 1, position.row});
            suspects.push_back({position.column + 1, position.row - 1});
        } else if (position.column > 4) {
            suspects.push_back({position.column - 1, position.row});
            suspects.push_back({position.column - 1, position.row + 1});
            suspects.push_back({position.column + 1, position.row});
            suspects.push_back({position.column + 1, position.row - 1});
        } else if (position.column < 4) {
            suspects.push_back({position.column + 1, position.row});
            suspects.push_back({position.column + 1, position.row + 1});
            suspects.push_back({position.column - 1, position.row});
            suspects.push_back({position.column - 1, position.row - 1});
        }

        for (auto position : suspects) {
            if (position.isValid())
                closerPositions.push_back(position);
        }

        return closerPositions;
    }
}

auto findFurtherPositions(Position position) -> std::vector<Position> {
    auto furtherPositions = std::vector<Position>();
    auto suspects = std::vector<Position>();

    if (position.column <= 2) {
        suspects.push_back({position.column, position.row - 2});
        suspects.push_back({position.column, position.row + 2});

        suspects.push_back({position.column - 1, position.row + 1});
        suspects.push_back({position.column - 1, position.row - 2});

        suspects.push_back({position.column - 2, position.row});
        suspects.push_back({position.column - 2, position.row - 1});
        suspects.push_back({position.column - 2, position.row - 2});

        suspects.push_back({position.column + 1, position.row - 1});
        suspects.push_back({position.column + 1, position.row + 2});

        suspects.push_back({position.column + 2, position.row});
        suspects.push_back({position.column + 2, position.row + 1});
        suspects.push_back({position.column + 2, position.row + 2});
    }
    else if (position.column == 3) {
        suspects.push_back({position.column, position.row - 2});
        suspects.push_back({position.column, position.row + 2});

        suspects.push_back({position.column - 1, position.row + 1});
        suspects.push_back({position.column - 1, position.row - 2});

        suspects.push_back({position.column - 2, position.row});
        suspects.push_back({position.column - 2, position.row - 1});
        suspects.push_back({position.column - 2, position.row - 2});

        suspects.push_back({position.column + 1, position.row - 1});
        suspects.push_back({position.column + 1, position.row + 2});

        suspects.push_back({position.column + 2, position.row - 1});
        suspects.push_back({position.column + 2, position.row});
        suspects.push_back({position.column + 2, position.row + 1});
    }
    else if (position.column == 4) {
        suspects.push_back({position.column, position.row - 2});
        suspects.push_back({position.column, position.row + 2});

        suspects.push_back({position.column - 1, position.row + 1});
        suspects.push_back({position.column - 1, position.row - 2});

        suspects.push_back({position.column - 2, position.row});
        suspects.push_back({position.column - 2, position.row - 1});
        suspects.push_back({position.column - 2, position.row - 2});

        suspects.push_back({position.column + 1, position.row - 2});
        suspects.push_back({position.column + 1, position.row + 1});

        suspects.push_back({position.column + 2, position.row - 2});
        suspects.push_back({position.column + 2, position.row - 1});
        suspects.push_back({position.column + 2, position.row });
    }
    else if (position.column == 5) {
        suspects.push_back({position.column, position.row - 2});
        suspects.push_back({position.column, position.row + 2});

        suspects.push_back({position.column - 1, position.row - 1});
        suspects.push_back({position.column - 1, position.row + 2});

        suspects.push_back({position.column - 2, position.row - 1});
        suspects.push_back({position.column - 2, position.row});
        suspects.push_back({position.column - 2, position.row + 1});

        suspects.push_back({position.column + 1, position.row - 2});
        suspects.push_back({position.column + 1, position.row + 1});

        suspects.push_back({position.column + 2, position.row - 2});
        suspects.push_back({position.column + 2, position.row - 1});
        suspects.push_back({position.column + 2, position.row});
    }
    else if (position.column >= 6) {
        suspects.push_back({position.column, position.row - 2});
        suspects.push_back({position.column, position.row + 2});

        suspects.push_back({position.column - 1, position.row - 1});
        suspects.push_back({position.column - 1, position.row + 2});

        suspects.push_back({position.column - 2, position.row});
        suspects.push_back({position.column - 2, position.row + 1});
        suspects.push_back({position.column - 2, position.row + 2});

        suspects.push_back({position.column + 1, position.row - 2});
        suspects.push_back({position.column + 1, position.row + 1});

        suspects.push_back({position.column + 2, position.row - 2});
        suspects.push_back({position.column + 2, position.row - 1});
        suspects.push_back({position.column + 2, position.row});
    }
    for (auto suspect: suspects) {
        if (suspect.isValid())
            furtherPositions.push_back(suspect);
    }
    return furtherPositions;
}


