#include <iostream>
//plansza


#define SIZE_OF_BOARD 8
#define ROWS 8
#define COLUMNS 8
#define LINES_WITH_PAWNS_AT_START 3
struct Spot 
{
   int value = 0;// 1 = biale, 0 = puste, -1 = czarne
};

//miejsce board[0][0] ozaczna lewy dolny rog planszy
//
Spot* board[ROWS][COLUMNS];

void board_init(void);
void print_board(void);

int main()
{
    board_init;
    std::cout << "DUPA";
    print_board;
    return 0;
}

void board_init()
{
    //Tutuaj petla do zainicjalizowania 6 linijjek startowych w ktrych znajdują się pionki początkowe
    for(int i = 0;i < LINES_WITH_PAWNS_AT_START; i++)
    {
        for(int j = 0; j < SIZE_OF_BOARD; j++)
        {
            //ustaw biale
            Spot withe_pawn;
            withe_pawn.value = 1;
            Spot black_pawn;
            black_pawn.value = -1;
            if(j % 2 == 0)//pole parzyste
            {
                *board[i][j] = withe_pawn;
            }
            else // pole nieparzyste
            {
                *board[SIZE_OF_BOARD - i][j] = black_pawn;
            }
            //ustaw czarne
        }
    }
}

void print_board()
{
    for (int i=0;i<ROWS;i++)
    {
        for (int j=0;j<COLUMNS;j++)
        {
            std::cout << board[i][j].value << " ";
        }
        std::cout << std::endl;
    }
}