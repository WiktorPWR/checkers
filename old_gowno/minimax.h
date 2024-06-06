#ifndef MINIMAX_H
#define MINIMAX_H

#include "board.h"


int minimax(int depth, bool is_maximizing_player);
int evaluate_board();
void best_solution_max_player();
#endif // MINIMAX_H
