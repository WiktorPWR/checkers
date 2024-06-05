#include <iostream>

#define SIZE_OF_BOARD 8
#define ROWS SIZE_OF_BOARD
#define COLUMNS SIZE_OF_BOARD
#define LINES_WITH_PAWNS_AT_START 3

struct Spot 
{
    // int row;
    // int column;
    int value = 0; // 1 = białe, 0 = puste, -1 = czarne
};

struct Actions 
{
    bool can_move_left;
    bool can_move_right;
    bool can_capture_left;
    bool can_capture_right;
};

// Miejsce board[0][0] oznacza lewy górny róg planszy
Spot* board[ROWS][COLUMNS];

void board_init();
void print_board();
Actions pawn_actions(int row, int column, bool team);

int main()
{
    board_init();
    std::cout << "Plansza:" << std::endl;
    print_board();
    
    // Testowanie funkcji pawn_actions
    board[3][0]->value = -1;
    board[3][2]->value = -1;
    int row = 5, column = 2;
    bool team = false; // true - białe, false czarne
    Actions result = pawn_actions(row, column, team);
    std::cout << "Wynik dla pola (" << row << ", " << column << ") i druzyny " << (team ? "bialej" : "czarnej") << ":\n";
    std::cout << "WARTOSC POLA " << board[row][column]->value << std::endl;
    std::cout << "Mozna ruszyc w lewo: " << result.can_move_left << "\n";
    std::cout << "Mozna ruszyc w prawo: " << result.can_move_right << "\n";
    std::cout << "Mozna bic w lewo: " << result.can_capture_left << "\n";
    std::cout << "Mozna bic w prawo: " << result.can_capture_right << "\n";

    return 0;
}

void board_init()
{
    // Inicjalizacja wskaźników do Spot
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            board[i][j] = new Spot(); // Przydzielenie pamięci dla każdego pola
            // board[i][j]->row = i;
            // board[i][j]->column = j;
        }
    }
    
    // Ustawianie początkowych pionków
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            if (i < LINES_WITH_PAWNS_AT_START)
            {
                // Ustaw białe pionki na dolnych liniach
                if ((i + j) % 2 != 0) // tylko na czarnych polach
                {
                    board[i][j]->value = 1;
                }
            }
            else if (i >= ROWS - LINES_WITH_PAWNS_AT_START)
            {
                // Ustaw czarne pionki na górnych liniach
                if ((i + j) % 2 != 0) // tylko na czarnych polach
                {
                    board[i][j]->value = -1;
                }
            }
            else
            {
                // Pozostaw puste miejsca w środkowej części
                board[i][j]->value = 0;
            }
        }
    }
}

//team jezeli biale to true jezeli czarne to false
Actions pawn_actions(int row, int column, bool team)
{
    Actions actions = {false, false, false, false};

    // sprawdzanie czy nalezy do poprawnej druzyny
    if (team && board[row][column]->value == 1)
    {
        //sprawdzenie czy pionek nie  jest na colummnie 0 czyli na lewej krawedzi planszy
        if(column > 0 && board[row + 1][column - 1]->value == 0)
        {
            actions.can_move_left = true;
        }
        //sprawdzenie czy pionek nie jest na colummnie 7 czyli na prawej krawedzi planszy
        if(column < 7 && board[row + 1][column + 1]->value == 0)
        {
            actions.can_move_right = true;
        }
        //bicie w lewo
        if(column > 1 && board[row + 1][column - 1]->value == -1 && board[row + 2][column - 2]->value == 0)
        {
            actions.can_capture_left = true;
        }
        //bivie w prawo
        if(column < 6 && board[row + 1][column + 1]->value == -1 && board[row + 2][column + 2]->value == 0)
        {
            actions.can_capture_right = true;
        }

    }
    else if (!team && board[row][column]->value == -1)
    {
       //sprawdzenie czy pionek nie  jest na colummnie 0 czyli na lewej krawedzi planszy
        if(column > 0 && board[row - 1 ][column - 1]->value == 0)
        {
            actions.can_move_left = true;
        }
        //sprawdzenie czy pionek nie jest na colummnie 7 czyli na prawej krawedzi planszy
        if(column < 7 && board[row - 1][column + 1]->value == 0)
        {
            actions.can_move_right = true;
        }
        //bicie w lewo
        if(column > 1 && board[row - 1][column - 1]->value == 1 && board[row - 2][column - 2]->value == 0)
        {
            actions.can_capture_left = true;
        }
        //bivie w prawo
        if(column < 6 && board[row - 1][column + 1]->value == 1 && board[row - 2][column + 2]->value == 0)
        {
            actions.can_capture_right = true;
        }
    }

    return actions;
}

void print_board()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            std::cout << board[i][j]->value << " ";
        }
        std::cout << std::endl;
    }
}
