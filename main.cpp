#include <iostream>
#include <vector>
#include <limits>

#define SIZE_OF_BOARD 8
#define ROWS SIZE_OF_BOARD
#define COLUMNS SIZE_OF_BOARD
#define LINES_WITH_PAWNS_AT_START 3

static int white_pawns = 12;
static int black_pawns = 12;

struct Spot 
{
    int value = 0; // 1 = białe, 0 = puste, -1 = czarne
};

struct Actions 
{
    bool can_move_left;
    bool can_move_right;
    bool can_capture_left;
    bool can_capture_right;
};

struct MoveOption
{
    int row;
    int column;
    bool L_R; // true for right, false for left
};

struct CaptureOption
{
    int row;
    int column;
    bool L_R; // true for right, false for left
};

Spot* board[ROWS][COLUMNS];

void board_init();
void print_board();
Actions pawn_actions(int row, int column, bool team);
void move_pawn(int row, int column, bool team, Actions result, bool L_R);
void strike_pawn(int row, int column, bool team, Actions result, bool L_R);
std::vector<CaptureOption> get_capture_options(bool team);
bool has_captures(bool team);

int main()
{
    board_init();
    std::cout << "Plansza:" << std::endl;
    print_board();

    bool team = true; // true for white, false for black

    while(white_pawns != 0 && black_pawns != 0)
    {
        bool valid_action = false;
        while (!valid_action)
        {
            std::cout << "Teraz ruch ma " << (team ? "bialy" : "czarny") << " team." << std::endl;
            std::cout << "DAWAJ JAKA AKCJE CHCESZ BYK 1.MOVE PAWN 2.STRIKE" << std::endl;
            int action;
            if (!(std::cin >> action))
            {
                std::cin.clear(); // clear the error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
                std::cout << "Nieprawidłowa akcja. Spróbuj ponownie." << std::endl;
                continue;
            }
            
            int row, column;
            bool L_R;

            switch (action)
            {
            case 1:
                if (has_captures(team))
                {
                    std::cout << "MUSISZ BIC" << std::endl;
                }
                else
                {
                    std::cout << "PODAJ LOKALIZACJE PIONKA najpierw wiersz nastepnie kolumne" << std::endl;
                    if (!(std::cin >> row >> column))
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Nieprawidłowa lokalizacja. Spróbuj ponownie." << std::endl;
                        break;
                    }
                    Actions result = pawn_actions(row, column, team);
                    std::cout << "Możliwe akcje:" << std::endl;
                    if (result.can_move_left)
                        std::cout << "1. Ruch w lewo" << std::endl;
                    if (result.can_move_right)
                        std::cout << "2. Ruch w prawo" << std::endl;
                    if (result.can_capture_left)
                        std::cout << "3. Bicie w lewo" << std::endl;
                    if (result.can_capture_right)
                        std::cout << "4. Bicie w prawo" << std::endl;
                    std::cout << "Wybierz akcję: ";
                    int chosen_action;
                    if (!(std::cin >> chosen_action))
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Nieprawidłowa akcja. Spróbuj ponownie." << std::endl;
                        break;
                    }
                    switch (chosen_action)
                    {
                    case 1:
                        if (result.can_move_left)
                        {
                            move_pawn(row, column, team, result, false);
                            valid_action = true;
                        }
                        else
                        {
                            std::cout << "Nieprawidłowa akcja. Spróbuj ponownie." << std::endl;
                        }
                        break;
                    case 2:
                        if (result.can_move_right)
                        {
                            move_pawn(row, column, team, result, true);
                            valid_action = true;
                        }
                        else
                        {
                            std::cout << "Nieprawidłowa akcja. Spróbuj ponownie." << std::endl;
                        }
                        break;
                    case 3:
                        if (result.can_capture_left)
                        {
                            strike_pawn(row, column, team, result, false);
                            valid_action = true;
                        }
                        else
                        {
                            std::cout << "Nieprawidłowa akcja. Spróbuj ponownie." << std::endl;
                        }
                        break;
                    case 4:
                        if (result.can_capture_right)
                        {
                            strike_pawn(row, column, team, result, true);
                            valid_action = true;
                        }
                        else
                        {
                            std::cout << "Nieprawidłowa akcja. Spróbuj ponownie." << std::endl;
                        }
                        break;
                    default:
                        std::cout << "Nieprawidłowa akcja. Spróbuj ponownie." << std::endl;
                        break;
                    }
                }
                break;

            case 2:
                std::cout << "Podaj lokalizacje pionka najpierw wiersz nastepnie kolumne" << std::endl;
                if (!(std::cin >> row >> column))
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Nieprawidłowa lokalizacja. Spróbuj ponownie." << std::endl;
                    break;
                }
                Actions result = pawn_actions(row, column, team);
                std::cout << "Możliwe akcje:" << std::endl;
                if (result.can_capture_left)
                    std::cout << "1. Bicie w lewo" << std::endl;
                if (result.can_capture_right)
                    std::cout << "2. Bicie w prawo" << std::endl;
                std::cout << "Wybierz akcję: ";
                int chosen_action;
                if (!(std::cin >> chosen_action))
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Nieprawidłowa akcja. Spróbuj ponownie." << std::endl;
                    break;
                }
                switch (chosen_action)
                {
                case 1:
                    if (result.can_capture_left)
                    {
                        strike_pawn(row, column, team, result, false);
                        valid_action = true;
                    }
                    else
                    {
                        std::cout << "Nieprawidłowa akcja. Spróbuj ponownie." << std::endl;
                    }
                    break;
                case 2:
                    if (result.can_capture_right)
                    {
                        strike_pawn(row, column, team, result, true);
                        valid_action = true;
                    }
                    else
                    {
                        std::cout << "Nieprawidłowa akcja. Spróbuj ponownie." << std::endl;
                    }
                    break;
                default:
                    std::cout << "Nieprawidłowa akcja. Spróbuj ponownie." << std::endl;
                    break;
                }
                break;
            }
        }

        // Switch teams for next turn
        team = !team;
        print_board();
    }

    return 0;
}

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
