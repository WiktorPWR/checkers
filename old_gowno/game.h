#ifndef GAME_H
#define GAME_H

#include "board.h"

class Game {
public:
    Game();
    void play();
private:
    Board board;
    bool playerTurn;
    void playerMove();
    void computerMove();
};

#endif
