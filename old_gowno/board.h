#ifndef BOARD_H
#define BOARD_H

#include <vector>

#define SIZE_OF_BOARD 8
#define ROWS SIZE_OF_BOARD
#define COLUMNS SIZE_OF_BOARD
#define LINES_WITH_PAWNS_AT_START 3

struct Spot
{
    int value = 0; // 1 = białe, 0 = puste, -1 = czarne, 2 = damka biała, -2 = damka czarna
};

struct Actions
{
    bool can_move_left;
    bool can_move_right;
    bool can_capture_left;
    bool can_capture_right;
    bool can_move_back_left;
    bool can_move_back_right;
    bool can_capture_back_left;
    bool can_capture_back_right;
};

struct Move
{
    int from_row;
    int from_col;
    int to_row;
    int to_col;
};

extern Spot* board[ROWS][COLUMNS];
extern int white_pawns;
extern int black_pawns;

void board_init();
void print_board();
Actions pawn_actions(int row, int column, bool team);
void move_pawn(int row, int column, bool team, Actions result, bool L_R, bool back);
void strike_pawn(int row, int column, bool team, Actions result, bool L_R, bool back);
bool can_continue_capture(int row, int column, bool team);
std::vector<Move> generate_all_captures(int row, int column, bool team);
std::vector<Move> has_captures_vector(bool team);
std::vector<Move> get_all_moves(bool team);
bool has_captures_bool(bool team);
bool game_over();
void handle_multiple_captures(int& row, int& column, bool team);


#endif
