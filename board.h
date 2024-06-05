#ifndef BOARD_H
#define BOARD_H

#include <vector>

#define SIZE_OF_BOARD 8
#define ROWS SIZE_OF_BOARD
#define COLUMNS SIZE_OF_BOARD
#define LINES_WITH_PAWNS_AT_START 3

extern int white_pawns;
extern int black_pawns;

struct Spot 
{
    int value; // 1 = białe, 0 = puste, -1 = czarne
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

struct Move
{
    int from_row;
    int from_col;
    int to_row;
    int to_col;
};

extern Spot* board[ROWS][COLUMNS];

void board_init();
void print_board();
Actions pawn_actions(int row, int column, bool team);
void move_pawn(int row, int column, bool team, Actions result, bool L_R);
void strike_pawn(int row, int column, bool team, Actions result, bool L_R);
std::vector<CaptureOption> get_capture_options(bool team);
bool has_captures(bool team);
bool game_over();
std::vector<Move> get_all_moves(bool team);

#endif // BOARD_H
