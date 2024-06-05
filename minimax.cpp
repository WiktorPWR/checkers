#include "../checkers/minimax.h"
#include <limits>
int minimax(int depth, bool is_maximizing_player)
{
    if (depth == 0 || game_over())
    {
        return evaluate_board();
    }

    std::vector<Move> moves = get_all_moves(is_maximizing_player);

    if (is_maximizing_player)
    {
        int max_eval = std::numeric_limits<int>::min();
        for (const Move& move : moves)
        {
            // Execute the move
            int from_value = board[move.from_row][move.from_col]->value;
            int to_value = board[move.to_row][move.to_col]->value;

            board[move.to_row][move.to_col]->value = from_value;
            board[move.from_row][move.from_col]->value = 0;

            int eval = minimax(depth - 1, false);

            // Undo the move
            board[move.from_row][move.from_col]->value = from_value;
            board[move.to_row][move.to_col]->value = to_value;

            max_eval = std::max(max_eval, eval);
        }
        return max_eval;
    }
    else
    {
        int min_eval = std::numeric_limits<int>::max();
        for (const Move& move : moves)
        {
            // Execute the move
            int from_value = board[move.from_row][move.from_col]->value;
            int to_value = board[move.to_row][move.to_col]->value;

            board[move.to_row][move.to_col]->value = from_value;
            board[move.from_row][move.from_col]->value = 0;

            int eval = minimax(depth - 1, true);

            // Undo the move
            board[move.from_row][move.from_col]->value = from_value;
            board[move.to_row][move.to_col]->value = to_value;

            min_eval = std::min(min_eval, eval);
        }
        return min_eval;
    }
}
