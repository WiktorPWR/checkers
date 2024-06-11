#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <utility>

class Board {
public:
    enum class Player { NONE, HUMAN, COMPUTER };
    Board();
    Board(const Board& other); // konstruktor kopiujący
    bool isGameOver() const;
    Player getWinner() const;
    void print() const;
    bool makeMove(Player player, int fromX, int fromY, int toX, int toY);
    std::pair<std::pair<int, int>, std::pair<int, int>> getBestMove();

private:
    std::vector<std::vector<Player>> board;
    int minimax(int depth, bool maximizingPlayer);
    int evaluate() const;
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> getPossibleMoves(Player player) const;
};

#endif
