#include "board.h"
#include <iostream>
#include <limits>

Board::Board() : board(8, std::vector<Player>(8, Player::NONE)) {
    for (int i = 0; i < 8; i++) {
        for (int j = (i % 2); j < 8; j += 2) {
            if (i < 3) board[i][j] = Player::COMPUTER;
            else if (i > 4) board[i][j] = Player::HUMAN;
        }
    }
}

Board::Board(const Board& other) : board(other.board) {}

bool Board::isGameOver() const {
    return getPossibleMoves(Player::HUMAN).empty() || getPossibleMoves(Player::COMPUTER).empty();
}

Board::Player Board::getWinner() const {
    if (getPossibleMoves(Player::HUMAN).empty()) return Player::COMPUTER;
    if (getPossibleMoves(Player::COMPUTER).empty()) return Player::HUMAN;
    return Player::NONE;
}

void Board::print() const {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            char c = '.';
            if (cell == Player::HUMAN) c = 'H';
            else if (cell == Player::COMPUTER) c = 'C';
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }
}

bool Board::makeMove(Player player, int fromX, int fromY, int toX, int toY) {
    if (board[fromX][fromY] != player || board[toX][toY] != Player::NONE) return false;
    int dx = toX - fromX;
    int dy = toY - fromY;
    if (abs(dx) != abs(dy) || abs(dx) > 2) return false;

    if (abs(dx) == 2) {
        int midX = (fromX + toX) / 2;
        int midY = (fromY + toY) / 2;
        if (board[midX][midY] == Player::NONE || board[midX][midY] == player) return false;
        board[midX][midY] = Player::NONE;
    }
    board[toX][toY] = player;
    board[fromX][fromY] = Player::NONE;
    return true;
}

std::pair<std::pair<int, int>, std::pair<int, int>> Board::getBestMove() {
    int bestValue = std::numeric_limits<int>::min();
    std::pair<std::pair<int, int>, std::pair<int, int>> bestMove;
    for (const auto& move : getPossibleMoves(Player::COMPUTER)) {
        Board tempBoard(*this); // używamy kopii planszy
        int fromX = move.first.first;
        int fromY = move.first.second;
        int toX = move.second.first;
        int toY = move.second.second;
        tempBoard.makeMove(Player::COMPUTER, fromX, fromY, toX, toY);
        int moveValue = tempBoard.minimax(3, false);
        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = move;
        }
    }
    return bestMove;
}

int Board::minimax(int depth, bool maximizingPlayer) {
    if (depth == 0 || isGameOver()) return evaluate();

    if (maximizingPlayer) {
        int maxEval = std::numeric_limits<int>::min();
        for (const auto& move : getPossibleMoves(Player::COMPUTER)) {
            Board tempBoard(*this); // używamy kopii planszy
            int fromX = move.first.first;
            int fromY = move.first.second;
            int toX = move.second.first;
            int toY = move.second.second;
            tempBoard.makeMove(Player::COMPUTER, fromX, fromY, toX, toY);
            int eval = tempBoard.minimax(depth - 1, false);
            maxEval = std::max(maxEval, eval);
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (const auto& move : getPossibleMoves(Player::HUMAN)) {
            Board tempBoard(*this); // używamy kopii planszy
            int fromX = move.first.first;
            int fromY = move.first.second;
            int toX = move.second.first;
            int toY = move.second.second;
            tempBoard.makeMove(Player::HUMAN, fromX, fromY, toX, toY);
            int eval = tempBoard.minimax(depth - 1, true);
            minEval = std::min(minEval, eval);
        }
        return minEval;
    }
}

int Board::evaluate() const {
    int score = 0;
    for (const auto& row : board) {
        for (const auto& cell : row) {
            if (cell == Player::HUMAN) score -= 1;
            if (cell == Player::COMPUTER) score += 1;
        }
    }
    return score;
}

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> Board::getPossibleMoves(Player player) const {
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> moves;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (board[x][y] == player) {
                for (int dx = -2; dx <= 2; dx++) {
                    for (int dy = -2; dy <= 2; dy++) {
                        int toX = x + dx;
                        int toY = y + dy;
                        if (toX >= 0 && toX < 8 && toY >= 0 && toY < 8) {
                            Board tempBoard(*this);
                            if (tempBoard.makeMove(player, x, y, toX, toY)) {
                                moves.push_back({{x, y}, {toX, toY}});
                            }
                        }
                    }
                }
            }
        }
    }
    return moves;
}
