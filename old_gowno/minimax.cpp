#include "../checkers/minimax.h"
#include "../checkers/board.h"
#include <limits>
#include <iostream>
#include <vector>


int minimax(int depth, bool is_maximizing_player)
{
    if (depth == 0 || game_over())
    {
        return evaluate_board();
    }

    std::vector<Move> moves = get_all_moves(is_maximizing_player);
    std::vector<Move> strikes = has_captures_vector(is_maximizing_player);

    if (!strikes.empty())
    {
        moves = strikes;
    }

    if (is_maximizing_player)
    {
        int max_eval = std::numeric_limits<int>::min();
        for (const Move& move : moves)
        {
            int from_value = board[move.from_row][move.from_col]->value;
            int to_value = board[move.to_row][move.to_col]->value;
            int captured_row = (move.from_row + move.to_row) / 2;
            int captured_col = (move.from_col + move.to_col) / 2;
            int captured_value = 0;

            board[move.to_row][move.to_col]->value = from_value;
            board[move.from_row][move.from_col]->value = 0;

            if (abs(move.to_row - move.from_row) == 2)
            {
                captured_value = board[captured_row][captured_col]->value;
                board[captured_row][captured_col]->value = 0;
                if (captured_value == 1 || captured_value == 2)
                    black_pawns--;
                else if (captured_value == -1 || captured_value == -2)
                    white_pawns--;
            }

            int eval = minimax(depth - 1, false);

            board[move.from_row][move.from_col]->value = from_value;
            board[move.to_row][move.to_col]->value = to_value;

            if (captured_value != 0)
            {
                board[captured_row][captured_col]->value = captured_value;
                if (captured_value == 1 || captured_value == 2)
                    black_pawns++;
                else if (captured_value == -1 || captured_value == -2)
                    white_pawns++;
            }

            max_eval = std::max(max_eval, eval);
        }
        return max_eval;
    }
    else
    {
        int min_eval = std::numeric_limits<int>::max();
        for (const Move& move : moves)
        {
            int from_value = board[move.from_row][move.from_col]->value;
            int to_value = board[move.to_row][move.to_col]->value;
            int captured_row = (move.from_row + move.to_row) / 2;
            int captured_col = (move.from_col + move.to_col) / 2;
            int captured_value = 0;

            board[move.to_row][move.to_col]->value = from_value;
            board[move.from_row][move.from_col]->value = 0;

            if (abs(move.to_row - move.from_row) == 2)
            {
                captured_value = board[captured_row][captured_col]->value;
                board[captured_row][captured_col]->value = 0;
                if (captured_value == 1 || captured_value == 2)
                    black_pawns--;
                else if (captured_value == -1 || captured_value == -2)
                    white_pawns--;
            }

            int eval = minimax(depth - 1, true);

            board[move.from_row][move.from_col]->value = from_value;
            board[move.to_row][move.to_col]->value = to_value;

            if (captured_value != 0)
            {
                board[captured_row][captured_col]->value = captured_value;
                if (captured_value == 1 || captured_value == 2)
                    black_pawns++;
                else if (captured_value == -1 || captured_value == -2)
                    white_pawns++;
            }

            min_eval = std::min(min_eval, eval);
        }
        return min_eval;
    }
}


int evaluate_board()
{
    int score = 0;

    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLUMNS; ++j)
        {
            int value = board[i][j]->value;
            if (value == 1) // Biały pionek
            {
                score += 1; // Punkt za biały pionek
                score += (i - 0); // Dodatkowy punkt za bliższą pozycję do przeciwnika
            }
            else if (value == -1) // Czarny pionek
            {
                score -= 1; // Punkt za czarny pionek
                score -= (ROWS - 1 - i); // Dodatkowy punkt za bliższą pozycję do przeciwnika
            }
            else if (value == 2) // Biała damka
            {
                score += 5; // Więcej punktów za białą damkę
            }
            else if (value == -2) // Czarna damka
            {
                score -= 5; // Więcej punktów za czarną damkę
            }
        }
    }

    return score;
}



void best_solution_max_player()
{
    Move best_move;
    int best_value = std::numeric_limits<int>::min();

    std::vector<Move> moves = get_all_moves(false);
    std::vector<Move> strikes = has_captures_vector(false);

    if (!strikes.empty())
    {
        moves = strikes;
    }

    for (const Move& move : moves)
    {
        int from_value = board[move.from_row][move.from_col]->value;
        int to_value = board[move.to_row][move.to_col]->value;
        int captured_row = (move.from_row + move.to_row) / 2;
        int captured_col = (move.from_col + move.to_col) / 2;
        int captured_value = 0;

        board[move.to_row][move.to_col]->value = from_value;
        board[move.from_row][move.from_col]->value = 0;

        if (abs(move.to_row - move.from_row) == 2)
        {
            captured_value = board[captured_row][captured_col]->value;
            board[captured_row][captured_col]->value = 0;
            if (captured_value == 1 || captured_value == 2)
                black_pawns--;
            else if (captured_value == -1 || captured_value == -2)
                white_pawns--;
        }

        int board_value = minimax(3, true);

        board[move.from_row][move.from_col]->value = from_value;
        board[move.to_row][move.to_col]->value = to_value;

        if (captured_value != 0)
        {
            board[captured_row][captured_col]->value = captured_value;
            if (captured_value == 1 || captured_value == 2)
                black_pawns++;
            else if (captured_value == -1 || captured_value == -2)
                white_pawns++;
        }

        if (board_value > best_value)
        {
            best_value = board_value;
            best_move = move;
        }
    }

    int from_value = board[best_move.from_row][best_move.from_col]->value;
    board[best_move.to_row][best_move.to_col]->value = from_value;
    board[best_move.from_row][best_move.from_col]->value = 0;

    if (abs(best_move.to_row - best_move.from_row) == 2)
    {
        int captured_row = (best_move.from_row + best_move.to_row) / 2;
        int captured_col = (best_move.from_col + best_move.to_col) / 2;
        board[captured_row][captured_col]->value = 0;
        black_pawns--;
    }

    // Obsługa wielokrotnego bicia
    if (can_continue_capture(best_move.to_row, best_move.to_col, false))
    {
        int new_row = best_move.to_row;
        int new_col = best_move.to_col;
        handle_multiple_captures(new_row, new_col, false);
    }

    // Sprawdzenie promocji na damkę
    if (from_value == -1 && best_move.to_row == 0)
    {
        board[best_move.to_row][best_move.to_col]->value = -2;
    }
}





