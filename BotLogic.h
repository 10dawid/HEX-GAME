#pragma once
#include "Board.h"

auto evaluate(State player, Board& board) -> int;
auto getPossibleMoves(State player, const Board&  board) -> std::vector<std::pair<Tile, Tile>>;
auto getBestMove(Board board, State player) -> std::pair<Tile, Tile>;
