#include <vector>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <chrono>

class Pawn
{
public:
    bool team; // 1 - white, 0 - black
    bool man_king = false; // false - man, true - king
    int position_x; // column
    int position_y; // row
    bool life_death = true; // life - true, death - false

    struct Actions
    {
        bool LEFT_MOVE_FORWARD = false;
        bool RIGHT_MOVE_FORWARD = false;
        bool LEFT_STRIKE_FORWARD = false;
        bool RIGHT_STRIKE_FORWARD = false;
        bool LEFT_MOVE_BACKWARD = false;
        bool RIGHT_MOVE_BACKWARD = false;
        bool LEFT_STRIKE_BACKWARD = false;
        bool RIGHT_STRIKE_BACKWARD = false;

        // Check if any action is true
        bool any_true() const
        {
            return LEFT_MOVE_FORWARD || RIGHT_MOVE_FORWARD || LEFT_STRIKE_FORWARD || RIGHT_STRIKE_FORWARD ||
                   LEFT_MOVE_BACKWARD || RIGHT_MOVE_BACKWARD || LEFT_STRIKE_BACKWARD || RIGHT_STRIKE_BACKWARD;
        }

        // Check if any move action is true
        bool move_true() const
        {
            return LEFT_MOVE_FORWARD || RIGHT_MOVE_FORWARD || LEFT_MOVE_BACKWARD || RIGHT_MOVE_BACKWARD;
        }

        // Check if any strike action is true
        bool strike_true() const
        {
            return LEFT_STRIKE_FORWARD || RIGHT_STRIKE_FORWARD || LEFT_STRIKE_BACKWARD || RIGHT_STRIKE_BACKWARD;
        }
    };

    Actions actions;

    // Constructor to initialize a pawn at a given position and team
    Pawn(int x, int y, bool TEAM)
    {
        position_x = x;
        position_y = y;
        team = TEAM;
    }
};

class Board
{
private:
    const int ROWS = 8; // Number of rows on the board
    const int COLUMNS = 8; // Number of columns on the board
    std::vector<std::vector<Pawn *>> board; // 2D board representation

    // Check if the pawn is an opponent
    bool is_opponent(Pawn* p, bool team) const
    {
        return p != nullptr && p->team != team;
    }

    // Move a pawn to a new position on the board
    void move_pawn(Pawn &pawn, int new_y, int new_x)
    {
        board[new_y][new_x] = &pawn; // new position
        board[pawn.position_y][pawn.position_x] = nullptr; // old position
        pawn.position_y = new_y;
        pawn.position_x = new_x;
    }

    // Check if a move is possible
    bool can_move(Pawn &pawn, int new_y, int new_x) const
    {
        return is_valid_position(new_x, new_y) && board[new_y][new_x] == nullptr;
    }

    // Check if a strike is possible
    bool can_strike(Pawn &pawn, int middle_y, int middle_x, int new_y, int new_x) const
    {
        return is_valid_position(new_x, new_y) &&
               board[new_y][new_x] == nullptr &&
               is_opponent(board[middle_y][middle_x], pawn.team);
    }

public:
    std::vector<Pawn> pawns; // List of all pawns on the board

    // Enumeration of possible user decisions
    enum USER_DECISION
    {
        LEFT_MOVE_FORWARD = 0,
        RIGHT_MOVE_FORWARD = 1,
        LEFT_MOVE_BACKWARD = 2,
        RIGHT_MOVE_BACKWARD = 3,
        LEFT_STRIKE_FORWARD = 4,
        RIGHT_STRIKE_FORWARD = 5,
        LEFT_STRIKE_BACKWARD = 6,
        RIGHT_STRIKE_BACKWARD = 7,
    };

    // Constructor to initialize the board with pawns
    Board()
    {
        for (int i = 0; i < ROWS; i++)
        {
            std::vector<Pawn *> row(COLUMNS, nullptr); // Initialize row with null pointers
            for (int j = 0; j < COLUMNS; j++)
            {
                if (i < 3)
                {
                    if ((i + j) % 2 != 0) // Only place pawns on alternating squares
                    {
                        pawns.push_back(Pawn(j, i, true)); // Add a white pawn
                        row[j] = &pawns.back();
                    }
                }
                else if (i >= ROWS - 3)
                {
                    if ((i + j) % 2 != 0)
                    {
                        pawns.push_back(Pawn(j, i, false)); // Add a black pawn
                        row[j] = &pawns.back();
                    }
                }
            }
            board.push_back(row);
        }
    }

    // Copy constructor for deep copy
    Board(const Board &other) : pawns(other.pawns), board(ROWS, std::vector<Pawn*>(COLUMNS, nullptr)) 
    {
        // Update board with new pawn positions
        for (int i = 0; i < pawns.size(); ++i)
        {
            if (pawns[i].life_death)
            {
                board[pawns[i].position_y][pawns[i].position_x] = &pawns[i];
            }
        }
    }

    // Update the board after a move
    void update_board()
    {
        // Clear the board
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS; j++)
            {
                board[i][j] = nullptr;
            }
        }

        // Place pawns in their current positions
        for (auto &pawn : pawns)
        {
            if (pawn.life_death)
            {
                board[pawn.position_y][pawn.position_x] = &pawn;
            }
        }
    }

    // Check if a pawn should be promoted to a king
    void chec_king(Pawn &pawn)
    {
        if (pawn.team)
        {
            if (pawn.position_y == 7)
            {
                pawn.man_king = true;
            }
        }
        else
        {
            if (pawn.position_y == 0)
            {
                pawn.man_king = true;
            }
        }
    }

    // Check if a left forward move is possible
    bool chec_left_forward(Pawn &pawn)
    {
        return can_move(pawn, pawn.team ? pawn.position_y + 1 : pawn.position_y - 1, pawn.position_x - 1);
    }

    // Check if a right forward move is possible
    bool chec_right_forward(Pawn &pawn)
    {
        return can_move(pawn, pawn.team ? pawn.position_y + 1 : pawn.position_y - 1, pawn.position_x + 1);
    }

    // Check if a left backward move is possible (for kings)
    bool chec_left_backward(Pawn &pawn)
    {
        return pawn.man_king && can_move(pawn, pawn.team ? pawn.position_y - 1 : pawn.position_y + 1, pawn.position_x - 1);
    }

    // Check if a right backward move is possible (for kings)
    bool chec_right_backward(Pawn &pawn)
    {
        return pawn.man_king && can_move(pawn, pawn.team ? pawn.position_y - 1 : pawn.position_y + 1, pawn.position_x + 1);
    }

    // Check if a left forward strike is possible
    bool chec_strike_left_forward(Pawn &pawn)
    {
        int middle_y = pawn.team ? pawn.position_y + 1 : pawn.position_y - 1;
        int middle_x = pawn.position_x - 1;
        int new_y = pawn.team ? pawn.position_y + 2 : pawn.position_y - 2;
        int new_x = pawn.position_x - 2;
        return can_strike(pawn, middle_y, middle_x, new_y, new_x);
    }

    // Check if a right forward strike is possible
    bool chec_strike_right_forward(Pawn &pawn)
    {
        int middle_y = pawn.team ? pawn.position_y + 1 : pawn.position_y - 1;
        int middle_x = pawn.position_x + 1;
        int new_y = pawn.team ? pawn.position_y + 2 : pawn.position_y - 2;
        int new_x = pawn.position_x + 2;
        return can_strike(pawn, middle_y, middle_x, new_y, new_x);
    }

    // Check if a left backward strike is possible (for kings)
    bool chec_strike_left_backward(Pawn &pawn)
    {
        int middle_y = pawn.team ? pawn.position_y - 1 : pawn.position_y + 1;
        int middle_x = pawn.position_x - 1;
        int new_y = pawn.team ? pawn.position_y - 2 : pawn.position_y + 2;
        int new_x = pawn.position_x - 2;
        return can_strike(pawn, middle_y, middle_x, new_y, new_x);
    }

    // Check if a right backward strike is possible (for kings)
    bool chec_strike_right_backward(Pawn &pawn)
    {
        int middle_y = pawn.team ? pawn.position_y - 1 : pawn.position_y + 1;
        int middle_x = pawn.position_x + 1;
        int new_y = pawn.team ? pawn.position_y - 2 : pawn.position_y + 2;
        int new_x = pawn.position_x + 2;
        return can_strike(pawn, middle_y, middle_x, new_y, new_x);
    }

    // Check and update the actions available to a pawn
    void chec_action(Pawn &pawn)
    {
        if (!pawn.life_death) return;
        pawn.actions.LEFT_MOVE_FORWARD = chec_left_forward(pawn);
        pawn.actions.RIGHT_MOVE_FORWARD = chec_right_forward(pawn);
        pawn.actions.LEFT_MOVE_BACKWARD = chec_left_backward(pawn);
        pawn.actions.RIGHT_MOVE_BACKWARD = chec_right_backward(pawn);
        pawn.actions.LEFT_STRIKE_FORWARD = chec_strike_left_forward(pawn);
        pawn.actions.RIGHT_STRIKE_FORWARD = chec_strike_right_forward(pawn);
        pawn.actions.LEFT_STRIKE_BACKWARD = chec_strike_left_backward(pawn);
        pawn.actions.RIGHT_STRIKE_BACKWARD = chec_strike_right_backward(pawn);
        chec_king(pawn);
    }

    // Move pawn left forward
    void move_left(Pawn &pawn) {
        int new_y = pawn.team ? pawn.position_y + 1 : pawn.position_y - 1;
        int new_x = pawn.position_x - 1;
        if (pawn.actions.LEFT_MOVE_FORWARD && can_move(pawn, new_y, new_x)) {
            move_pawn(pawn, new_y, new_x);
        }
    }

    // Move pawn right forward
    void move_right(Pawn &pawn) {
        int new_y = pawn.team ? pawn.position_y + 1 : pawn.position_y - 1;
        int new_x = pawn.position_x + 1;
        if (pawn.actions.RIGHT_MOVE_FORWARD && can_move(pawn, new_y, new_x)) {
            move_pawn(pawn, new_y, new_x);
        }
    }

    // Move pawn left backward (for kings)
    void move_left_backwards(Pawn &pawn)
    {
        if (pawn.man_king) {
            int new_y = pawn.team ? pawn.position_y - 1 : pawn.position_y + 1;
            int new_x = pawn.position_x - 1;
            if (pawn.actions.LEFT_MOVE_BACKWARD && can_move(pawn, new_y, new_x)) {
                move_pawn(pawn, new_y, new_x);
            }
        }
    }

    // Move pawn right backward (for kings)
    void move_right_backwards(Pawn &pawn)
    {
        if (pawn.man_king) {
            int new_y = pawn.team ? pawn.position_y - 1 : pawn.position_y + 1;
            int new_x = pawn.position_x + 1;
            if (pawn.actions.RIGHT_MOVE_BACKWARD && can_move(pawn, new_y, new_x)) {
                move_pawn(pawn, new_y, new_x);
            }
        }
    }

    // Perform left forward strike
    void strike_left(Pawn &pawn) {
        int middle_y = pawn.team ? pawn.position_y + 1 : pawn.position_y - 1;
        int middle_x = pawn.position_x - 1;
        int new_y = pawn.team ? pawn.position_y + 2 : pawn.position_y - 2;
        int new_x = pawn.position_x - 2;
        if (pawn.actions.LEFT_STRIKE_FORWARD && can_strike(pawn, middle_y, middle_x, new_y, new_x)) {
            move_pawn(pawn, new_y, new_x);
            board[middle_y][middle_x]->life_death = false; // Capture opponent pawn
            board[middle_y][middle_x] = nullptr;
        } else if (pawn.man_king && pawn.actions.LEFT_STRIKE_BACKWARD && can_strike(pawn, middle_y, middle_x, new_y, new_x)) {
            move_pawn(pawn, new_y, new_x);
            board[middle_y][middle_x]->life_death = false; // Capture opponent pawn
            board[middle_y][middle_x] = nullptr;
        }
    }

    // Perform right forward strike
    void strike_right(Pawn &pawn) {
        int middle_y = pawn.team ? pawn.position_y + 1 : pawn.position_y - 1;
        int middle_x = pawn.position_x + 1;
        int new_y = pawn.team ? pawn.position_y + 2 : pawn.position_y - 2;
        int new_x = pawn.position_x + 2;
        if (pawn.actions.RIGHT_STRIKE_FORWARD && can_strike(pawn, middle_y, middle_x, new_y, new_x)) {
            move_pawn(pawn, new_y, new_x);
            board[middle_y][middle_x]->life_death = false; // Capture opponent pawn
            board[middle_y][middle_x] = nullptr;
        } else if (pawn.man_king && pawn.actions.RIGHT_STRIKE_BACKWARD && can_strike(pawn, middle_y, middle_x, new_y, new_x)) {
            move_pawn(pawn, new_y, new_x);
            board[middle_y][middle_x]->life_death = false; // Capture opponent pawn
            board[middle_y][middle_x] = nullptr;
        }
    }

    // Perform left backward strike (for kings)
    void strike_left_backwards(Pawn &pawn) {
        int middle_y = pawn.team ? pawn.position_y - 1 : pawn.position_y + 1;
        int middle_x = pawn.position_x - 1;
        int new_y = pawn.team ? pawn.position_y - 2 : pawn.position_y + 2;
        int new_x = pawn.position_x - 2;
        if (pawn.actions.LEFT_STRIKE_BACKWARD && can_strike(pawn, middle_y, middle_x, new_y, new_x)) {
            move_pawn(pawn, new_y, new_x);
            board[middle_y][middle_x]->life_death = false; // Capture opponent pawn
            board[middle_y][middle_x] = nullptr;
        }
    }

    // Perform right backward strike (for kings)
    void strike_right_backwards(Pawn &pawn) {
        int middle_y = pawn.team ? pawn.position_y - 1 : pawn.position_y + 1;
        int middle_x = pawn.position_x + 1;
        int new_y = pawn.team ? pawn.position_y - 2 : pawn.position_y + 2;
        int new_x = pawn.position_x + 2;
        if (pawn.actions.RIGHT_STRIKE_BACKWARD && can_strike(pawn, middle_y, middle_x, new_y, new_x)) {
            move_pawn(pawn, new_y, new_x);
            board[middle_y][middle_x]->life_death = false; // Capture opponent pawn
            board[middle_y][middle_x] = nullptr;
        }
    }

    // Perform the chosen move for the pawn
    void move_man(Pawn &pawn, USER_DECISION decision)
    {
        chec_action(pawn); // Update pawn actions before making a move
        try
        {
            switch (decision)
            {
            case USER_DECISION::LEFT_MOVE_FORWARD:
                move_left(pawn);
                break;
            case USER_DECISION::LEFT_MOVE_BACKWARD:
                move_left_backwards(pawn);
                break;
            case USER_DECISION::RIGHT_MOVE_FORWARD:
                move_right(pawn);
                break;
            case USER_DECISION::RIGHT_MOVE_BACKWARD:
                move_right_backwards(pawn);
                break;
            case USER_DECISION::LEFT_STRIKE_FORWARD:
                strike_left(pawn);
                break;
            case USER_DECISION::LEFT_STRIKE_BACKWARD:
                strike_left_backwards(pawn);
                break;
            case USER_DECISION::RIGHT_STRIKE_FORWARD:
                strike_right(pawn);
                break;
            case USER_DECISION::RIGHT_STRIKE_BACKWARD:
                strike_right_backwards(pawn);
                break;
            default:
                std::cout << "Invalid move" << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in move_man: " << e.what() << std::endl;
        }
        update_board(); // Update the board after the move
    }

    // Print the current state of the board
    void print_board()
    {
        std::cout << "   ";
        for (int j = 0; j < COLUMNS; j++)
        {
            std::cout << "  " << j << " ";
        }
        std::cout << std::endl;

        for (int i = 0; i < ROWS; i++)
        {
            std::cout << " " << i << " ";
            for (int j = 0; j < COLUMNS; j++)
            {
                if (board[i][j] == nullptr)
                {
                    std::cout << "  0 ";
                }
                else if (board[i][j]->team)
                {
                    std::cout << "  1 ";
                }
                else
                {
                    std::cout << " -1 ";
                }
            }
            std::cout << std::endl;
        }
    }

    // Print the details of each pawn for debugging purposes
    void debug_print_pawns()
    {
        for (int i = 0; i < pawns.size(); ++i)
        {
            std::cout << "Pawn " << i << ": (" << pawns[i].position_x << ", " << pawns[i].position_y << ") "
                      << "Team: " << (pawns[i].team ? "White" : "Black") << " "
                      << "Alive: " << (pawns[i].life_death ? "Yes" : "No") << std::endl;
        }
    }

    // Check if the game is over
    bool game_over()
    {
        int white_points = 12;
        int black_points = -12;
        for (int i = 0; i < pawns.size(); i++)
        {
            if (pawns[i].team && pawns[i].life_death == false)
            {
                white_points--;
            }
            else if (!pawns[i].team && pawns[i].life_death == false)
            {
                black_points++;
            }
        }

        if (white_points == 0 || black_points == 0)
        {
            return true;
        }
        return false;
    }

    // Evaluate the board and return a score based on the state
    int evaluate_board(bool team)
{
    int white_points = 12;
    int black_points = -12;
    int king_bonus = 3; // Bonus for becoming a king
    int capture_bonus = 2; // Bonus for capturing an opponent's piece
    int avoid_loss_penalty = 2; // Penalty for losing a piece

    // Calculate points for each pawn and check for possible strikes and losses in one iteration
    for ( Pawn& pawn : pawns)
    {
        if (!pawn.life_death)
        {
            if (pawn.team)
            {
                white_points--;
            }
            else
            {
                black_points++;
            }
            continue; // Skip further checks for dead pawns
        }

        if (pawn.man_king)
        {
            if (pawn.team)
            {
                white_points += king_bonus;
            }
            else
            {
                black_points -= king_bonus;
            }
        }

        // Check possible strikes for both teams
        chec_action(pawn);
        if (pawn.actions.strike_true())
        {
            if (pawn.team == team)
            {
                if (team)
                {
                    white_points += capture_bonus;
                }
                else
                {
                    black_points -= capture_bonus;
                }
            }
            else
            {
                if (team)
                {
                    white_points -= avoid_loss_penalty;
                }
                else
                {
                    black_points += avoid_loss_penalty;
                }
            }
        }
    }

    // Return points based on team
    return team ? white_points : black_points;
}


    // Generate a list of pawns that can move
    std::vector<int> generate_moves(bool my_team)
    {
        std::vector<int> pawns_with_moves;
        for (int i = 0; i < pawns.size(); i++)
        {
            if (pawns[i].team == my_team)
            {
                chec_action(pawns[i]);
                if (pawns[i].actions.any_true())
                {
                    pawns_with_moves.push_back(i);
                }
            }
        }
        return pawns_with_moves;
    }


std::tuple<int, Board::USER_DECISION, int, bool> minimax(int depth, bool maximizingPlayer, bool team)
{
    if (depth == 0 || game_over())
    {
        int eval = evaluate_board(team);
        return {eval, LEFT_MOVE_FORWARD, -1, false}; // Added a flag to indicate no valid move
    }

    std::vector<int> pawns_id = generate_moves(team);
    Board::USER_DECISION bestMove;
    int bestPawnId;
    bool move_found = false;

    std::vector<std::tuple<int, Board::USER_DECISION, int>> captures;
    std::vector<std::tuple<int, Board::USER_DECISION, int>> moves;

    for (int id : pawns_id)
    {
        if (id < 0 || id >= pawns.size()) continue; // Ensure the ID is within valid range
        Pawn &pawn = pawns[id];
        if (!pawn.life_death) continue; // Skip pawns that are not alive

        chec_action(pawn);

        if (pawn.actions.strike_true())
        {
            if (pawn.actions.LEFT_STRIKE_FORWARD) captures.push_back({id, LEFT_STRIKE_FORWARD, 0});
            if (pawn.actions.RIGHT_STRIKE_FORWARD) captures.push_back({id, RIGHT_STRIKE_FORWARD, 0});
            if (pawn.actions.LEFT_STRIKE_BACKWARD) captures.push_back({id, LEFT_STRIKE_BACKWARD, 0});
            if (pawn.actions.RIGHT_STRIKE_BACKWARD) captures.push_back({id, RIGHT_STRIKE_BACKWARD, 0});
        }
        else
        {
            if (pawn.actions.LEFT_MOVE_FORWARD) moves.push_back({id, LEFT_MOVE_FORWARD, 0});
            if (pawn.actions.RIGHT_MOVE_FORWARD) moves.push_back({id, RIGHT_MOVE_FORWARD, 0});
            if (pawn.actions.LEFT_MOVE_BACKWARD) moves.push_back({id, LEFT_MOVE_BACKWARD, 0});
            if (pawn.actions.RIGHT_MOVE_BACKWARD) moves.push_back({id, RIGHT_MOVE_BACKWARD, 0});
        }
    }

    // Prioritize captures
    auto &action_list = !captures.empty() ? captures : moves;

    if (maximizingPlayer)
    {
        int maxEval = std::numeric_limits<int>::min();
        for (auto &[id, move, dummy] : action_list)
        {
            try
            {
                Board copy(*this);
                copy.move_man(copy.pawns[id], move);
                copy.update_board();
                int eval = std::get<0>(copy.minimax(depth - 1, false, !team));
                
                // Evaluate the board after making a move
                int current_eval = evaluate_board(team);
                eval += current_eval;

                if (eval > maxEval)
                {
                    maxEval = eval;
                    bestMove = move;
                    bestPawnId = id;
                    move_found = true; // Indicate a valid move was found
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error in minimax maximizing: " << e.what() << std::endl;
            }
        }
        return {maxEval, bestMove, bestPawnId, move_found};
    }
    else
    {
        int minEval = std::numeric_limits<int>::max();
        for (auto &[id, move, dummy] : action_list)
        {
            try
            {
                Board copy(*this);
                copy.move_man(copy.pawns[id], move);
                copy.update_board();
                int eval = std::get<0>(copy.minimax(depth - 1, true, !team));
                
                // Evaluate the board after making a move
                int current_eval = evaluate_board(team);
                eval -= current_eval;

                if (eval < minEval)
                {
                    minEval = eval;
                    bestMove = move;
                    bestPawnId = id;
                    move_found = true; // Indicate a valid move was found
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error in minimax minimizing: " << e.what() << std::endl;
            }
        }
        return {minEval, bestMove, bestPawnId, move_found};
    }
}

// Find the best move for the AI using the minimax algorithm
std::pair<Board::USER_DECISION, int> find_best_move(bool team, int depth)
{
    auto [eval, move, id, move_found] = minimax(depth, true, team);

    if (!move_found)
    {
        std::cerr << "No optimal move found. Selecting a fallback move." << std::endl;

        std::vector<int> pawns_id = generate_moves(team);
        for (int id : pawns_id)
        {
            if (id < 0 || id >= pawns.size()) continue; // Ensure the ID is within valid range
            Pawn &pawn = pawns[id];
            if (!pawn.life_death) continue; // Skip pawns that are not alive

            chec_action(pawn);

            std::vector<Board::USER_DECISION> possible_moves;
            if (pawn.actions.LEFT_MOVE_FORWARD) possible_moves.push_back(LEFT_MOVE_FORWARD);
            if (pawn.actions.RIGHT_MOVE_FORWARD) possible_moves.push_back(RIGHT_MOVE_FORWARD);
            if (pawn.actions.LEFT_MOVE_BACKWARD) possible_moves.push_back(LEFT_MOVE_BACKWARD);
            if (pawn.actions.RIGHT_MOVE_BACKWARD) possible_moves.push_back(RIGHT_MOVE_BACKWARD);
            if (pawn.actions.LEFT_STRIKE_FORWARD) possible_moves.push_back(LEFT_STRIKE_FORWARD);
            if (pawn.actions.RIGHT_STRIKE_FORWARD) possible_moves.push_back(RIGHT_STRIKE_FORWARD);
            if (pawn.actions.LEFT_STRIKE_BACKWARD) possible_moves.push_back(LEFT_STRIKE_BACKWARD);
            if (pawn.actions.RIGHT_STRIKE_BACKWARD) possible_moves.push_back(RIGHT_STRIKE_BACKWARD);

            if (!possible_moves.empty())
            {
                return {possible_moves[0], id}; // Return the first available move
            }
        }

        // If no moves are found (which theoretically should not happen)
        std::cerr << "No fallback move found either!" << std::endl;
        return {LEFT_MOVE_FORWARD, -1}; // Default to an invalid move
    }

    return {move, id};
}

    // Get the pawn at a specific position
    Pawn* get_pawn(int x, int y) const
    {
        if (is_valid_position(x, y))
        {
            return board[y][x];
        }
        return nullptr;
    }

    // Check if a position is valid
    bool is_valid_position(int x, int y) const
    {
        return x >= 0 && x < COLUMNS && y >= 0 && y < ROWS;
    }

    // Check if there is a mandatory capture for a team
    bool has_mandatory_capture(bool team) const
    {
        for (const Pawn &pawn : pawns)
        {
            if (pawn.team == team && pawn.life_death)
            {
                if (pawn.actions.strike_true())
                {
                    return true;
                }
            }
        }
        return false;
    }

    // Get a list of mandatory captures for a pawn
    std::vector<USER_DECISION> get_mandatory_captures(Pawn &pawn) const
    {
        std::vector<USER_DECISION> captures;
        if (pawn.actions.LEFT_STRIKE_FORWARD) captures.push_back(LEFT_STRIKE_FORWARD);
        if (pawn.actions.RIGHT_STRIKE_FORWARD) captures.push_back(RIGHT_STRIKE_FORWARD);
        if (pawn.actions.LEFT_STRIKE_BACKWARD) captures.push_back(LEFT_STRIKE_BACKWARD);
        if (pawn.actions.RIGHT_STRIKE_BACKWARD) captures.push_back(RIGHT_STRIKE_BACKWARD);
        return captures;
    }
};


int main()
{
    Board board;
    board.print_board(); // Print the initial board state

    // Automatic first user move
    Pawn &first_pawn = *board.get_pawn(1, 2); // Get pawn at coordinates (1, 2)
    board.move_man(first_pawn, Board::RIGHT_MOVE_FORWARD); // Make the move to the right
    board.print_board(); // Print the board state after the first move
    //board.update_board();

    bool game_over = false; // Game over flag
    std::vector<long long> ai_thinking_times; // Vector to store AI thinking times

    while (!game_over)
    {
        // List all pawns that can move for the user
        std::cout << "Your pawns that can move:\n";
        for (auto &pawn : board.pawns)
        {
            if (pawn.team && pawn.life_death)
            {
                board.chec_action(pawn);
                std::cout << "Pawn at (" << pawn.position_x << ", " << pawn.position_y << ") can ";
                if (pawn.actions.LEFT_MOVE_FORWARD) std::cout << "LEFT_MOVE_FORWARD ";
                if (pawn.actions.RIGHT_MOVE_FORWARD) std::cout << "RIGHT_MOVE_FORWARD ";
                if (pawn.actions.LEFT_MOVE_BACKWARD) std::cout << "LEFT_MOVE_BACKWARD ";
                if (pawn.actions.RIGHT_MOVE_BACKWARD) std::cout << "RIGHT_MOVE_BACKWARD ";
                if (pawn.actions.LEFT_STRIKE_FORWARD) std::cout << "LEFT_STRIKE_FORWARD ";
                if (pawn.actions.RIGHT_STRIKE_FORWARD) std::cout << "RIGHT_STRIKE_FORWARD ";
                if (pawn.actions.LEFT_STRIKE_BACKWARD) std::cout << "LEFT_STRIKE_BACKWARD ";
                if (pawn.actions.RIGHT_STRIKE_BACKWARD) std::cout << "RIGHT_STRIKE_BACKWARD ";
                std::cout << std::endl;
            }
        }

        // Check if there is a mandatory capture
        bool mandatory_capture = board.has_mandatory_capture(true);
        if (mandatory_capture)
        {
            std::cout << "You have a mandatory capture!" << std::endl;
        }

        // Get user input for pawn coordinates
        int x, y;
        std::cout << "Enter the coordinates (x y) of the pawn you want to move: ";
        std::cin >> x >> y;

        // Validate user input
        bool valid_move = board.is_valid_position(x, y) && board.get_pawn(x, y) != nullptr && board.get_pawn(x, y)->team;
        if (valid_move)
        {
            Pawn &pawn = *board.get_pawn(x, y);
            board.chec_action(pawn);
            valid_move = valid_move && pawn.actions.any_true();
            if (mandatory_capture)
            {
                valid_move = valid_move && pawn.actions.strike_true();
            }
        }

        while (!valid_move)
        {
            std::cout << "Invalid coordinates, not your pawn, no possible moves, or not a pawn with a mandatory capture. Enter again (x y): ";
            std::cin >> x >> y;

            valid_move = board.is_valid_position(x, y) && board.get_pawn(x, y) != nullptr && board.get_pawn(x, y)->team;
            if (valid_move)
            {
                Pawn &pawn = *board.get_pawn(x, y);
                board.chec_action(pawn);
                valid_move = valid_move && pawn.actions.any_true();
                if (mandatory_capture)
                {
                    valid_move = valid_move && pawn.actions.strike_true();
                }
            }
        }

        Pawn &pawn = *board.get_pawn(x, y); // Get the selected pawn
        board.chec_action(pawn); // Update the actions for the selected pawn

        std::vector<Board::USER_DECISION> possible_moves;
        if (mandatory_capture)
        {
            possible_moves = board.get_mandatory_captures(pawn); // Get mandatory captures if any
        }
        else
        {
            // Get all possible moves if no mandatory capture
            if (pawn.actions.LEFT_MOVE_FORWARD) possible_moves.push_back(Board::LEFT_MOVE_FORWARD);
            if (pawn.actions.RIGHT_MOVE_FORWARD) possible_moves.push_back(Board::RIGHT_MOVE_FORWARD);
            if (pawn.actions.LEFT_MOVE_BACKWARD) possible_moves.push_back(Board::LEFT_MOVE_BACKWARD);
            if (pawn.actions.RIGHT_MOVE_BACKWARD) possible_moves.push_back(Board::RIGHT_MOVE_BACKWARD);
            if (pawn.actions.LEFT_STRIKE_FORWARD) possible_moves.push_back(Board::LEFT_STRIKE_FORWARD);
            if (pawn.actions.RIGHT_STRIKE_FORWARD) possible_moves.push_back(Board::RIGHT_STRIKE_FORWARD);
            if (pawn.actions.LEFT_STRIKE_BACKWARD) possible_moves.push_back(Board::LEFT_STRIKE_BACKWARD);
            if (pawn.actions.RIGHT_STRIKE_BACKWARD) possible_moves.push_back(Board::RIGHT_STRIKE_BACKWARD);
        }

        // Print all available moves for the selected pawn
        std::cout << "Available moves:\n";
        for (int i = 0; i < possible_moves.size(); i++)
        {
            std::cout << i << ": ";
            switch (possible_moves[i])
            {
            case Board::LEFT_MOVE_FORWARD:
                std::cout << "LEFT_MOVE_FORWARD";
                break;
            case Board::RIGHT_MOVE_FORWARD:
                std::cout << "RIGHT_MOVE_FORWARD";
                break;
            case Board::LEFT_MOVE_BACKWARD:
                std::cout << "LEFT_MOVE_BACKWARD";
                break;
            case Board::RIGHT_MOVE_BACKWARD:
                std::cout << "RIGHT_MOVE_BACKWARD";
                break;
            case Board::LEFT_STRIKE_FORWARD:
                std::cout << "LEFT_STRIKE_FORWARD";
                break;
            case Board::RIGHT_STRIKE_FORWARD:
                std::cout << "RIGHT_STRIKE_FORWARD";
                break;
            case Board::LEFT_STRIKE_BACKWARD:
                std::cout << "LEFT_STRIKE_BACKWARD";
                break;
            case Board::RIGHT_STRIKE_BACKWARD:
                std::cout << "RIGHT_STRIKE_BACKWARD";
                break;
            }
            std::cout << std::endl;
        }

        // Get user input for the move choice
        int move_choice;
        std::cout << "Enter the number of the move you want to make: ";
        std::cin >> move_choice;

        while (move_choice < 0 || move_choice >= possible_moves.size())
        {
            std::cout << "Invalid choice. Enter again: ";
            std::cin >> move_choice;
        }

        board.move_man(pawn, possible_moves[move_choice]); // Perform the chosen move
        board.print_board(); // Print the board state after the move

        game_over = board.game_over(); // Check if the game is over
        if (game_over)
        {
            std::cout << "Game over! White wins!" << std::endl;
            break;
        }

        // AI turn
        std::cout << "AI is thinking..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        auto best_move = board.find_best_move(false, 6); // Adjust depth as necessary
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        ai_thinking_times.push_back(duration); // Store the duration
        std::cout << "Czas wykonania: " << duration << " mikrosekund" << std::endl;
        std::cout << "AI's best move: " << best_move.first << " by pawn ID: " << best_move.second << std::endl;

        if (best_move.second != -1) {
            board.move_man(board.pawns[best_move.second], best_move.first); // Perform AI's best move
        }
        board.print_board(); // Print the board state after AI's move

        game_over = board.game_over(); // Check if the game is over
        if (game_over)
        {
            std::cout << "Game over! Black wins!" << std::endl;
            break;
        }
    }

    // Print AI thinking times at the end of the game
    std::cout << "AI thinking times (in microseconds) for each move:" << std::endl;
    for (const auto& time : ai_thinking_times)
    {
        std::cout << time << std::endl;
    }

    return 0;
}