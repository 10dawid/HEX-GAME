#include "Board.h"
#include "Tile.h"



auto evaluate(State player, Board& board) -> int {

    auto score = getScore(board.map);
    auto result = 0;
    result += 50 * (player == State::Player1 ? score.first : score.second);

    auto playerPawns = (player == State::Player1 ? getPlayerOneOptions(board.map) : getPlayerTwoOptions(board.map));
    auto centerPawns = 0;
    for (auto pawn : playerPawns) {
        if (pawn.position.column == 2 || pawn.position.column == 6) {
            if (pawn.position.row >= 2 && pawn.position.row <= 4)
                centerPawns += 1;
        }
        else if (pawn.position.column == 3 || pawn.position.column == 5) {
            if (pawn.position.row >= 2 && pawn.position.row <= 5)
                centerPawns += 1;
        }
        else if (pawn.position.column == 4) {
            if (pawn.position.row >= 2 && pawn.position.row <= 6)
                centerPawns += 1;
        }
    }
    result += centerPawns * 5;

    auto solidPawns = 0;
    for (auto pawn : playerPawns) {
        auto supports = 0;
        for (auto neighbour : pawn.closerPositions) {
            if (pawn.state == board.map[neighbour.column][neighbour.row].state)
                supports += 1;
            if (supports >= 3) {
                solidPawns += 1;
                break;
            }
        }
    }

    result += 15 * solidPawns;





    return result;
}

auto getPossibleMoves(State player, const Board&  board) -> std::vector<std::pair<Tile, Tile>> {
    auto playerPawns = std::vector<Tile>();
    if (player == State::Player1)
        playerPawns = getPlayerOneOptions(board.map);
    else
        playerPawns = getPlayerTwoOptions(board.map);

    auto result = std::vector<std::pair<Tile,Tile>>();

    for (auto const& fromTile : playerPawns) {
        for (auto const& targetTile : fromTile.closerPositions)
            result.push_back({fromTile, board.map[targetTile.column][targetTile.row]});
        for (auto const& targetTile : fromTile.furtherPositions)
            result.push_back({fromTile, board.map[targetTile.column][targetTile.row]});
    }

    return result;

}


auto getBestMove(Board board, State player) -> std::pair<Tile, Tile>{
    auto pawns = (player == State::Player1 ? getPlayerOneOptions(board.map) : getPlayerTwoOptions(board.map));
    auto moves = getPossibleMoves(player, board);
    auto bestMove = moves[0];
    auto simulatedMap = makeMove(board.map, bestMove.first, bestMove.second);
    auto simulatedBoard = Board();
    simulatedBoard.map = simulatedMap;
    auto bestValue = evaluate(player, simulatedBoard);

    for (auto & move : moves) {
        simulatedMap = makeMove(board.map, move.first, move.second);
        simulatedBoard.map = simulatedMap;

        auto value = evaluate(player, simulatedBoard);

        if (value >= bestValue) {
            bestValue = value;
            bestMove = move;
        }
    }

    return bestMove;

}