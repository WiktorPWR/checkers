#include "game.h"
#include <iostream>

Game::Game() : playerTurn(true) {}

void Game::play() {
    while (!board.isGameOver()) {
        board.print();
        if (playerTurn) {
            playerMove();
        } else {
            computerMove();
        }
        playerTurn = !playerTurn;
    }
    board.print();
    if (board.getWinner() == Board::Player::NONE) {
        std::cout << "It's a draw!" << std::endl;
    } else {
        std::cout << (board.getWinner() == Board::Player::HUMAN ? "Human" : "Computer") << " wins!" << std::endl;
    }
}

void Game::playerMove() {
    int fromX, fromY, toX, toY;
    std::cout << "Enter your move (fromX fromY toX toY): ";
    std::cin >> fromX >> fromY >> toX >> toY;
    while (!board.makeMove(Board::Player::HUMAN, fromX, fromY, toX, toY)) {
        std::cout << "Invalid move. Enter again: ";
        std::cin >> fromX >> fromY >> toX >> toY;
    }
}

void Game::computerMove() {
    auto move = board.getBestMove();
    board.makeMove(Board::Player::COMPUTER, move.first.first, move.first.second, move.second.first, move.second.second);
    std::cout << "Computer moved from (" << move.first.first << "," << move.first.second << ") to (" << move.second.first << "," << move.second.second << ")" << std::endl;
}
