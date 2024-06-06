#include "../checkers/board.h"
#include <iostream>

int white_pawns = 12;
int black_pawns = 12;

Spot* board[ROWS][COLUMNS];

void board_init()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            board[i][j] = new Spot();
        }
    }

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            if (i < LINES_WITH_PAWNS_AT_START)
            {
                if ((i + j) % 2 != 0)
                {
                    board[i][j]->value = 1;
                }
            }
            else if (i >= ROWS - LINES_WITH_PAWNS_AT_START)
            {
                if ((i + j) % 2 != 0)
                {
                    board[i][j]->value = -1;
                }
            }
            else
            {
                board[i][j]->value = 0;
            }
        }
    }
}

Actions pawn_actions(int row, int column, bool team)
{
    Actions actions = {false, false, false, false, false, false, false, false};

    if (team && (board[row][column]->value == 1 || board[row][column]->value == 2))
    {
        if (column > 0 && board[row + 1][column - 1]->value == 0)
        {
            actions.can_move_left = true;
        }
        if (column < 7 && board[row + 1][column + 1]->value == 0)
        {
            actions.can_move_right = true;
        }
        if (column > 1 && board[row + 1][column - 1]->value < 0 && board[row + 2][column - 2]->value == 0)
        {
            actions.can_capture_left = true;
        }
        if (column < 6 && board[row + 1][column + 1]->value < 0 && board[row + 2][column + 2]->value == 0)
        {
            actions.can_capture_right = true;
        }
        if (board[row][column]->value == 2)
        {
            if (column > 0 && board[row - 1][column - 1]->value == 0)
            {
                actions.can_move_back_left = true;
            }
            if (column < 7 && board[row - 1][column + 1]->value == 0)
            {
                actions.can_move_back_right = true;
            }
            if (column > 1 && board[row - 1][column - 1]->value < 0 && board[row - 2][column - 2]->value == 0)
            {
                actions.can_capture_back_left = true;
            }
            if (column < 6 && board[row - 1][column + 1]->value < 0 && board[row - 2][column + 2]->value == 0)
            {
                actions.can_capture_back_right = true;
            }
        }
    }
    else if (!team && (board[row][column]->value == -1 || board[row][column]->value == -2))
    {
        if (column > 0 && board[row - 1][column - 1]->value == 0)
        {
            actions.can_move_left = true;
        }
        if (column < 7 && board[row - 1][column + 1]->value == 0)
        {
            actions.can_move_right = true;
        }
        if (column > 1 && board[row - 1][column - 1]->value > 0 && board[row - 2][column - 2]->value == 0)
        {
            actions.can_capture_left = true;
        }
        if (column < 6 && board[row - 1][column + 1]->value > 0 && board[row - 2][column + 2]->value == 0)
        {
            actions.can_capture_right = true;
        }
        if (board[row][column]->value == -2)
        {
            if (column > 0 && board[row + 1][column - 1]->value == 0)
            {
                actions.can_move_back_left = true;
            }
            if (column < 7 && board[row + 1][column + 1]->value == 0)
            {
                actions.can_move_back_right = true;
            }
            if (column > 1 && board[row + 1][column - 1]->value > 0 && board[row + 2][column - 2]->value == 0)
            {
                actions.can_capture_back_left = true;
            }
            if (column < 6 && board[row + 1][column + 1]->value > 0 && board[row + 2][column + 2]->value == 0)
            {
                actions.can_capture_back_right = true;
            }
        }
    }

    return actions;
}

void move_pawn(int row, int column, bool team, Actions result, bool L_R, bool back)
{
    if (result.can_capture_left || result.can_capture_right || result.can_capture_back_left || result.can_capture_back_right)
    {
        std::cout << "MUSISZ BIC" << std::endl;
        strike_pawn(row, column, team, result, L_R, back);
        return;
    }
    if (team)
    {
        if ((L_R && result.can_move_right) || (!L_R && result.can_move_left) || (back && ((L_R && result.can_move_back_right) || (!L_R && result.can_move_back_left))))
        {
            int new_row = back ? row - 1 : row + 1;
            int new_col = L_R ? column + 1 : column - 1;
            board[new_row][new_col]->value = board[row][column]->value;
            board[row][column]->value = 0;

            // Check for promotion to queen
            if (new_row == 7)
            {
                board[new_row][new_col]->value = 2;
            }
        }
        else
        {
            std::cout << "Nie mozna ruszyc" << std::endl;
        }
    }
    else
    {
        if ((L_R && result.can_move_right) || (!L_R && result.can_move_left) || (back && ((L_R && result.can_move_back_right) || (!L_R && result.can_move_back_left))))
        {
            int new_row = back ? row + 1 : row - 1;
            int new_col = L_R ? column + 1 : column - 1;
            board[new_row][new_col]->value = board[row][column]->value;
            board[row][column]->value = 0;

            // Check for promotion to queen
            if (new_row == 0)
            {
                board[new_row][new_col]->value = -2;
            }
        }
        else
        {
            std::cout << "Nie mozna ruszyc" << std::endl;
        }
    }
}

void strike_pawn(int row, int column, bool team, Actions result, bool L_R, bool back)
{
    if (team)
    {
        int new_row, new_col, captured_row, captured_col;
        if ((L_R && result.can_capture_right) || (!L_R && result.can_capture_left) || (back && ((L_R && result.can_capture_back_right) || (!L_R && result.can_capture_back_left))))
        {
            if (back)
            {
                new_row = row - 2;
                captured_row = row - 1;
            }
            else
            {
                new_row = row + 2;
                captured_row = row + 1;
            }
            new_col = L_R ? column + 2 : column - 2;
            captured_col = L_R ? column + 1 : column - 1;

            board[new_row][new_col]->value = board[row][column]->value;
            board[row][column]->value = 0;
            board[captured_row][captured_col]->value = 0;
            black_pawns--;

            // Check for promotion to queen
            if (new_row == 7)
            {
                board[new_row][new_col]->value = 2;
            }
        }
        else
        {
            std::cout << "Nie mozna bic" << std::endl;
        }
    }
    else
    {
        int new_row, new_col, captured_row, captured_col;
        if ((L_R && result.can_capture_right) || (!L_R && result.can_capture_left) || (back && ((L_R && result.can_capture_back_right) || (!L_R && result.can_capture_back_left))))
        {
            if (back)
            {
                new_row = row + 2;
                captured_row = row + 1;
            }
            else
            {
                new_row = row - 2;
                captured_row = row - 1;
            }
            new_col = L_R ? column + 2 : column - 2;
            captured_col = L_R ? column + 1 : column - 1;

            board[new_row][new_col]->value = board[row][column]->value;
            board[row][column]->value = 0;
            board[captured_row][captured_col]->value = 0;
            white_pawns--;

            // Check for promotion to queen
            if (new_row == 0)
            {
                board[new_row][new_col]->value = -2;
            }
        }
        else
        {
            std::cout << "Nie mozna bic" << std::endl;
        }
    }
}

// int evaluate_board()
// {
//     int score = 0;

//     for (int i = 0; i < ROWS; ++i)
//     {
//         for (int j = 0; j < COLUMNS; ++j)
//         {
//             if (board[i][j]->value == 1)
//             {
//                 score += 1; // białe pionki
//             }
//             else if (board[i][j]->value == -1)
//             {
//                 score -= 1; // czarne pionki
//             }
//             else if (board[i][j]->value == 2)
//             {
//                 score += 3; // białe damki
//             }
//             else if (board[i][j]->value == -2)
//             {
//                 score -= 3; // czarne damki
//             }
//         }
//     }

//     return score;
// }

bool game_over()
{
    if (white_pawns == 0 || black_pawns == 0)
    {
        return true;
    }

    // Sprawdź, czy biały gracz ma możliwe ruchy
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLUMNS; ++j)
        {
            if (board[i][j]->value == 1 || board[i][j]->value == 2)
            {
                Actions actions = pawn_actions(i, j, true);
                if (actions.can_move_left || actions.can_move_right || actions.can_capture_left || actions.can_capture_right || actions.can_move_back_left || actions.can_move_back_right || actions.can_capture_back_left || actions.can_capture_back_right)
                {
                    return false;
                }
            }
        }
    }

    // Sprawdź, czy czarny gracz ma możliwe ruchy
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLUMNS; ++j)
        {
            if (board[i][j]->value == -1 || board[i][j]->value == -2)
            {
                Actions actions = pawn_actions(i, j, false);
                if (actions.can_move_left || actions.can_move_right || actions.can_capture_left || actions.can_capture_right || actions.can_move_back_left || actions.can_move_back_right || actions.can_capture_back_left || actions.can_capture_back_right)
                {
                    return false;
                }
            }
        }
    }

    // Jeśli żaden z graczy nie ma możliwych ruchów, gra jest zakończona
    return true;
}

void print_board()
{
    // Print column numbers
    std::cout << "   ";
    for (int j = 0; j < COLUMNS; j++)
    {
        std::cout << " " << j << "  ";
    }
    std::cout << std::endl;

    for (int i = 0; i < ROWS; i++)
    {
        // Print row number
        std::cout << i << " ";

        for (int j = 0; j < COLUMNS; j++)
        {
            if(board[i][j]->value == -1 || board[i][j]->value == -2)
            {
                std::cout << " " <<  board[i][j]->value << " ";
            }
            else
            {
                std::cout << "  " <<  board[i][j]->value << " ";
            }
        }
        std::cout << std::endl;
    }
}

std::vector<Move> get_all_moves(bool team)
{
    std::vector<Move> all_moves;

    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLUMNS; ++j)
        {
            if ((team && (board[i][j]->value == 1 || board[i][j]->value == 2)) || (!team && (board[i][j]->value == -1 || board[i][j]->value == -2)))
            {
                Actions actions = pawn_actions(i, j, team);

                if (actions.can_move_left)
                {
                    all_moves.push_back({i, j, team ? i + 1 : i - 1, j - 1});
                }
                if (actions.can_move_right)
                {
                    all_moves.push_back({i, j, team ? i + 1 : i - 1, j + 1});
                }
                if (actions.can_move_back_left)
                {
                    all_moves.push_back({i, j, team ? i - 1 : i + 1, j - 1});
                }
                if (actions.can_move_back_right)
                {
                    all_moves.push_back({i, j, team ? i - 1 : i + 1, j + 1});
                }
                if (actions.can_capture_left)
                {
                    all_moves.push_back({i, j, team ? i + 2 : i - 2, j - 2});
                }
                if (actions.can_capture_right)
                {
                    all_moves.push_back({i, j, team ? i + 2 : i - 2, j + 2});
                }
                if (actions.can_capture_back_left)
                {
                    all_moves.push_back({i, j, team ? i - 2 : i + 2, j - 2});
                }
                if (actions.can_capture_back_right)
                {
                    all_moves.push_back({i, j, team ? i - 2 : i + 2, j + 2});
                }
            }
        }
    }

    std::cout << "Dla drużyny " << (team ? "białych" : "czarnych") << " jest " << all_moves.size() << " możliwych akcji." << std::endl;
    return all_moves;
}


bool has_captures_bool(bool team)
{
    std::vector<Move> all_moves = get_all_moves(team);
    for (const Move& move : all_moves)
    {
        if (abs(move.to_row - move.from_row) == 2)
        {
            return true;
        }
    }

    return false;
}

std::vector<Move> generate_all_captures(int row, int column, bool team)
{
    std::vector<Move> captures;

    while (can_continue_capture(row, column, team))
    {
        Actions actions = pawn_actions(row, column, team);
        int new_row, new_col, captured_row, captured_col;
        
        if (actions.can_capture_left)
        {
            new_row = team ? row + 2 : row - 2;
            captured_row = team ? row + 1 : row - 1;
            new_col = column - 2;
            captured_col = column - 1;
            captures.push_back({row, column, new_row, new_col});
        }
        else if (actions.can_capture_right)
        {
            new_row = team ? row + 2 : row - 2;
            captured_row = team ? row + 1 : row - 1;
            new_col = column + 2;
            captured_col = column + 1;
            captures.push_back({row, column, new_row, new_col});
        }
        else if (actions.can_capture_back_left)
        {
            new_row = team ? row - 2 : row + 2;
            captured_row = team ? row - 1 : row + 1;
            new_col = column - 2;
            captured_col = column - 1;
            captures.push_back({row, column, new_row, new_col});
        }
        else if (actions.can_capture_back_right)
        {
            new_row = team ? row - 2 : row + 2;
            captured_row = team ? row - 1 : row + 1;
            new_col = column + 2;
            captured_col = column + 1;
            captures.push_back({row, column, new_row, new_col});
        }
        else
        {
            break;
        }

        // Wykonanie bicia
        board[new_row][new_col]->value = board[row][column]->value;
        board[row][column]->value = 0;
        board[captured_row][captured_col]->value = 0;

        if (team)
        {
            black_pawns--;
        }
        else
        {
            white_pawns--;
        }

        // Aktualizacja pozycji pionka
        row = new_row;
        column = new_col;

        // Sprawdzenie promocji na damkę
        if (team && row == 7)
        {
            board[row][column]->value = 2;
            break;
        }
        else if (!team && row == 0)
        {
            board[row][column]->value = -2;
            break;
        }
    }

    return captures;
}

std::vector<Move> has_captures_vector(bool team)
{
    std::vector<Move> captures;

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            if ((team && board[i][j]->value > 0) || (!team && board[i][j]->value < 0))
            {
                std::vector<Move> temp_captures = generate_all_captures(i, j, team);
                captures.insert(captures.end(), temp_captures.begin(), temp_captures.end());
            }
        }
    }

    return captures;
}

void handle_multiple_captures(int& row, int& column, bool team)
{
    while (can_continue_capture(row, column, team))
    {
        Actions actions = pawn_actions(row, column, team);
        int new_row, new_col, captured_row, captured_col;
        
        // Priorytetowo wybieramy możliwe bicie (lewo -> prawo -> tył w lewo -> tył w prawo)
        if (actions.can_capture_left)
        {
            new_row = team ? row + 2 : row - 2;
            captured_row = team ? row + 1 : row - 1;
            new_col = column - 2;
            captured_col = column - 1;
        }
        else if (actions.can_capture_right)
        {
            new_row = team ? row + 2 : row - 2;
            captured_row = team ? row + 1 : row - 1;
            new_col = column + 2;
            captured_col = column + 1;
        }
        else if (actions.can_capture_back_left)
        {
            new_row = team ? row - 2 : row + 2;
            captured_row = team ? row - 1 : row + 1;
            new_col = column - 2;
            captured_col = column - 1;
        }
        else if (actions.can_capture_back_right)
        {
            new_row = team ? row - 2 : row + 2;
            captured_row = team ? row - 1 : row + 1;
            new_col = column + 2;
            captured_col = column + 1;
        }
        else
        {
            break;
        }

        // Wykonanie bicia
        board[new_row][new_col]->value = board[row][column]->value;
        board[row][column]->value = 0;
        board[captured_row][captured_col]->value = 0;
        
        if (team)
        {
            black_pawns--;
        }
        else
        {
            white_pawns--;
        }

        // Aktualizacja pozycji pionka
        row = new_row;
        column = new_col;

        // Sprawdzenie promocji na damkę
        if (team && row == 7)
        {
            board[row][column]->value = 2;
        }
        else if (!team && row == 0)
        {
            board[row][column]->value = -2;
        }
    }
}

