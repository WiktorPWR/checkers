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
            board[i][j]->value = 0; // Initialize with empty spots
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
        }
    }
}

Actions pawn_actions(int row, int column, bool team)
{
    Actions actions = {false, false, false, false};

    if (team && board[row][column]->value == 1)
    {
        if (column > 0 && board[row + 1][column - 1]->value == 0)
        {
            actions.can_move_left = true;
        }
        if (column < 7 && board[row + 1][column + 1]->value == 0)
        {
            actions.can_move_right = true;
        }
        if (column > 1 && board[row + 1][column - 1]->value == -1 && board[row + 2][column - 2]->value == 0)
        {
            actions.can_capture_left = true;
        }
        if (column < 6 && board[row + 1][column + 1]->value == -1 && board[row + 2][column + 2]->value == 0)
        {
            actions.can_capture_right = true;
        }
    }
    else if (!team && board[row][column]->value == -1)
    {
        if (column > 0 && board[row - 1][column - 1]->value == 0)
        {
            actions.can_move_left = true;
        }
        if (column < 7 && board[row - 1][column + 1]->value == 0)
        {
            actions.can_move_right = true;
        }
        if (column > 1 && board[row - 1][column - 1]->value == 1 && board[row - 2][column - 2]->value == 0)
        {
            actions.can_capture_left = true;
        }
        if (column < 6 && board[row - 1][column + 1]->value == 1 && board[row - 2][column + 2]->value == 0)
        {
            actions.can_capture_right = true;
        }
    }

    return actions;
}

void move_pawn(int row, int column, bool team, Actions result, bool L_R)
{
    if(result.can_capture_left || result.can_capture_right)
    {
        std::cout << "MUSISZ BIC" << std::endl;
        strike_pawn(row, column, team, result, L_R);       
        return;
    }
    if(team)
    {
        if(L_R && result.can_move_right)
        {
            board[row + 1][column + 1]->value = 1;
            board[row][column]->value = 0;
        }
        else if(!L_R && result.can_move_left)
        {
            board[row + 1][column - 1]->value = 1;
            board[row][column]->value = 0;
        }
        else
        {
            std::cout << "Nie mozna ruszyc" << std::endl;
        }
    }
    else
    {
        if(L_R && result.can_move_right)
        {
            board[row - 1][column + 1]->value = -1;
            board[row][column]->value = 0;
        }
        else if(!L_R && result.can_move_left)
        {
            board[row - 1][column - 1]->value = -1;
            board[row][column]->value = 0;
        }
        else
        {
            std::cout << "Nie mozna ruszyc" << std::endl;
        }
    }
}

void strike_pawn(int row, int column, bool team, Actions result, bool L_R)
{
    if(team)
    {
        if(L_R && result.can_capture_right)
        {
            board[row + 2][column + 2]->value = 1;
            board[row + 1][column + 1]->value = 0;
            board[row][column]->value = 0;
            black_pawns--;
        }
        else if(!L_R && result.can_capture_left)
        {
            board[row + 2][column - 2]->value = 1;
            board[row + 1][column - 1]->value = 0;
            board[row][column]->value = 0;
            black_pawns--;
        }
        else
        {
            std::cout << "Nie mozna bic" << std::endl;
        }
    }
    else
    {
        if(L_R && result.can_capture_right)
        {
            board[row - 2][column + 2]->value = -1;
            board[row - 1][column + 1]->value = 0;
            board[row][column]->value = 0;
            white_pawns--;
        }
        else if(!L_R && result.can_capture_left)
        {
            board[row - 2][column - 2]->value = -1;
            board[row - 1][column - 1]->value = 0;
            board[row][column]->value = 0;
            white_pawns--;
        }
        else
        {
            std::cout << "Nie mozna bic" << std::endl;
        }
    }
}

std::vector<CaptureOption> get_capture_options(bool team)
{
    std::vector<CaptureOption> captures;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            if ((team && board[i][j]->value == 1) || (!team && board[i][j]->value == -1))
            {
                Actions actions = pawn_actions(i, j, team);
                if (actions.can_capture_left)
                {
                    captures.push_back({i, j, false});
                }
                if (actions.can_capture_right)
                {
                    captures.push_back({i, j, true});
                }
            }
        }
    }
    return captures;
}

bool has_captures(bool team)
{
    return !get_capture_options(team).empty();
}

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
            if (board[i][j]->value == 1)
            {
                Actions actions = pawn_actions(i, j, true);
                if (actions.can_move_left || actions.can_move_right || actions.can_capture_left || actions.can_capture_right)
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
            if (board[i][j]->value == -1)
            {
                Actions actions = pawn_actions(i, j, false);
                if (actions.can_move_left || actions.can_move_right || actions.can_capture_left || actions.can_capture_right)
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
            if(board[i][j]->value == -1)
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
            if ((team && board[i][j]->value == 1) || (!team && board[i][j]->value == -1))
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
                if (actions.can_capture_left)
                {
                    all_moves.push_back({i, j, team ? i + 2 : i - 2, j - 2});
                }
                if (actions.can_capture_right)
                {
                    all_moves.push_back({i, j, team ? i + 2 : i - 2, j + 2});
                }
            }
        }
    }

    std::cout << "Dla drużyny " << (team ? "białych" : "czarnych") << " jest " << all_moves.size() << " możliwych akcji." << std::endl;
    return all_moves;
}
