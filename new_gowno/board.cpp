#include <vector>
#include <iostream>


class Pawn
{
    public:
        bool team;// 1 - whitem 0 black
        bool man_king = false;// false man, true king // zwykly pion false, damka true
        int position_x;// column
        int position_y;// row
        bool life_death = true;// life - true, death - false
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


            bool any_true() const {
            return LEFT_MOVE_FORWARD ||
               RIGHT_MOVE_FORWARD ||
               LEFT_STRIKE_FORWARD ||
               RIGHT_STRIKE_FORWARD ||
               LEFT_MOVE_BACKWARD ||
               RIGHT_MOVE_BACKWARD ||
               LEFT_STRIKE_BACKWARD ||
               RIGHT_STRIKE_BACKWARD;
            }
        
            bool move_true() const{
                return LEFT_MOVE_FORWARD ||
                RIGHT_MOVE_FORWARD ||
                LEFT_MOVE_BACKWARD ||
                RIGHT_MOVE_BACKWARD;
            }

            bool strike_true() const{
                return LEFT_STRIKE_FORWARD ||
                RIGHT_STRIKE_FORWARD ||
                LEFT_STRIKE_BACKWARD ||
               RIGHT_STRIKE_BACKWARD;
            }
        };

        Actions actions;
        
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
        const int ROWS = 8;
        const int COLUMNS = 8;
        std::vector<std::vector<Pawn*>> board;
        
        
    public:
        std::vector<Pawn> pawns;
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

        Board()
        {
            for (int i = 0; i < ROWS; i++)
            {
                std::vector<Pawn*> row(COLUMNS, nullptr); // Inicjalizujemy wiersz pustymi wskaźnikami
                for (int j = 0; j < COLUMNS; j++)
                {
                    if (i < 3)
                    {
                        if ((i + j) % 2 != 0)
                        {
                            pawns.push_back(Pawn(j, i, true));
                            row[j] = &pawns.back();
                        }
                    }
                    else if (i >= ROWS - 3)
                    {
                        if ((i + j) % 2 != 0)
                        {
                            pawns.push_back(Pawn(j, i, false));
                            row[j] = &pawns.back();
                        }
                    }
                }
                board.push_back(row);
            }
        }

        void chec_king(Pawn &pawn)
        {
            if(pawn.team)
            {
                if(pawn.position_y == 7)
                {
                    pawn.man_king = true;
                }
            }
            else
            {
                if(pawn.position_y == 0)
                {
                    pawn.man_king = true;
                }
            }
        }

        bool chec_left_forward(Pawn &pawn)
        {
            if (pawn.position_x > 0)
            {
                if (!pawn.team && pawn.position_y > 0 && board[pawn.position_y - 1][pawn.position_x - 1] == nullptr) // Białe pionki
                {
                    return true;
                }
                else if (pawn.team && pawn.position_y < ROWS - 1 && board[pawn.position_y + 1][pawn.position_x - 1] == nullptr) // Czarne pionki
                {
                    return true;
                }
            }
            return false;
        }

        bool chec_right_forward(Pawn &pawn)
        {
            if (pawn.position_x < COLUMNS - 1)
            {
                if (!pawn.team && pawn.position_y > 0 && board[pawn.position_y - 1][pawn.position_x + 1] == nullptr) // Białe pionki
                {
                    return true;
                }
                else if (pawn.team && pawn.position_y < ROWS - 1 && board[pawn.position_y + 1][pawn.position_x + 1] == nullptr) // Czarne pionki
                {
                    return true;
                }
            }
            return false;
        }

        bool chec_left_backward(Pawn &pawn)
        {
            if (pawn.position_x > 0 && pawn.man_king)
            {
                if (!pawn.team && pawn.position_y < ROWS - 1 && board[pawn.position_y + 1][pawn.position_x - 1] == nullptr) // Białe pionki
                {
                    return true;
                }
                else if (pawn.team && pawn.position_y > 0 && board[pawn.position_y - 1][pawn.position_x - 1] == nullptr) // Czarne pionki
                {
                    return true;
                }
            }
            return false;
        }

        bool chec_right_backward(Pawn &pawn)
        {
            if (pawn.position_x < COLUMNS - 1 && pawn.man_king)
            {
                if (!pawn.team && pawn.position_y < ROWS - 1 && board[pawn.position_y + 1][pawn.position_x + 1] == nullptr) // Białe pionki
                {
                    return true;
                }
                else if (pawn.team && pawn.position_y > 0 && board[pawn.position_y - 1][pawn.position_x + 1] == nullptr) // Czarne pionki
                {
                    return true;
                }
            }
            return false;
        }

        bool chec_strike_left_forward(Pawn &pawn)
        {
            if (pawn.position_x > 1)
            {
                if (!pawn.team && pawn.position_y > 1 && board[pawn.position_y - 1][pawn.position_x - 1] != nullptr && board[pawn.position_y - 1][pawn.position_x - 1]->team != pawn.team)
                {
                    if (board[pawn.position_y - 2][pawn.position_x - 2] == nullptr)
                    {
                        return true;
                    }
                }
                else if (pawn.team && pawn.position_y < ROWS - 2 && board[pawn.position_y + 1][pawn.position_x - 1] != nullptr && board[pawn.position_y + 1][pawn.position_x - 1]->team != pawn.team)
                {
                    if (board[pawn.position_y + 2][pawn.position_x - 2] == nullptr)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        bool chec_strike_right_forward(Pawn &pawn)
        {
            if (pawn.position_x < COLUMNS - 2)
            {
                if (!pawn.team && pawn.position_y > 1 && board[pawn.position_y - 1][pawn.position_x + 1] != nullptr && board[pawn.position_y - 1][pawn.position_x + 1]->team != pawn.team)
                {
                    if (board[pawn.position_y - 2][pawn.position_x + 2] == nullptr)
                    {
                        return true;
                    }
                }
                else if (pawn.team && pawn.position_y < ROWS - 2 && board[pawn.position_y + 1][pawn.position_x + 1] != nullptr && board[pawn.position_y + 1][pawn.position_x + 1]->team != pawn.team)
                {
                    if (board[pawn.position_y + 2][pawn.position_x + 2] == nullptr)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        bool chec_strike_left_backward(Pawn &pawn)
        {
            if (pawn.position_x > 1 && pawn.man_king)
            {
                if (!pawn.team && pawn.position_y < ROWS - 2 && board[pawn.position_y + 1][pawn.position_x - 1] != nullptr && board[pawn.position_y + 1][pawn.position_x - 1]->team != pawn.team)
                {
                    if (board[pawn.position_y + 2][pawn.position_x - 2] == nullptr)
                    {
                        return true;
                    }
                }
                else if (pawn.team && pawn.position_y > 1 && board[pawn.position_y - 1][pawn.position_x - 1] != nullptr && board[pawn.position_y - 1][pawn.position_x - 1]->team != pawn.team)
                {
                    if (board[pawn.position_y - 2][pawn.position_x - 2] == nullptr)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        bool chec_strike_right_backward(Pawn &pawn)
        {
            if (pawn.position_x < COLUMNS - 2 && pawn.man_king)
            {
                if (!pawn.team && pawn.position_y < ROWS - 2 && board[pawn.position_y + 1][pawn.position_x + 1] != nullptr && board[pawn.position_y + 1][pawn.position_x + 1]->team != pawn.team)
                {
                    if (board[pawn.position_y + 2][pawn.position_x + 2] == nullptr)
                    {
                        return true;
                    }
                }
                else if (pawn.team && pawn.position_y > 1 && board[pawn.position_y - 1][pawn.position_x + 1] != nullptr && board[pawn.position_y - 1][pawn.position_x + 1]->team != pawn.team)
                {
                    if (board[pawn.position_y - 2][pawn.position_x + 2] == nullptr)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        void chec_action(Pawn &pawn)
        {
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

        void move_left(Pawn &pawn)
        {
            if(!pawn.team && pawn.actions.LEFT_MOVE_FORWARD)
            {
                board[pawn.position_y - 1][pawn.position_x - 1] = &pawn;//nowa pozycja
                board[pawn.position_y][pawn.position_x] = nullptr;//stara pozcyja
                pawn.position_y = pawn.position_y  - 1;
                pawn.position_x = pawn.position_x - 1;
            }
            else if(pawn.team && pawn.actions.LEFT_MOVE_FORWARD)
            {
                board[pawn.position_y + 1][pawn.position_x - 1] = &pawn;//nowa pozycja
                board[pawn.position_y][pawn.position_x] = nullptr;//stara pozcyja
                pawn.position_y = pawn.position_y + 1;
                pawn.position_x = pawn.position_x - 1;
            }
        }

        void move_left_backwards(Pawn &pawn)
        {
            if(!pawn.team && pawn.actions.LEFT_MOVE_BACKWARD)
            {
                board[pawn.position_y + 1][pawn.position_x - 1] = &pawn; // nowa pozycja
                board[pawn.position_y][pawn.position_x] = nullptr; // stara pozycja
                pawn.position_y = pawn.position_y  + 1;
                pawn.position_x = pawn.position_x - 1;
            }
            else if(pawn.team && pawn.actions.LEFT_MOVE_BACKWARD)
            {
                board[pawn.position_y - 1][pawn.position_x - 1] = &pawn; // nowa pozycja
                board[pawn.position_y][pawn.position_x] = nullptr; // stara pozycja
                pawn.position_y = pawn.position_y  - 1;
                pawn.position_x = pawn.position_x - 1;
            }
        }

        void move_rigth(Pawn &pawn)
        {
            if(!pawn.team && pawn.actions.RIGHT_MOVE_FORWARD)
            {
                board[pawn.position_y - 1][pawn.position_x + 1] = &pawn;//nowa pozycja
                board[pawn.position_y][pawn.position_x] = nullptr;//stara pozcyja
                pawn.position_y = pawn.position_y - 1;
                pawn.position_x = pawn.position_x + 1;
            }
            else if(pawn.team && pawn.actions.RIGHT_MOVE_FORWARD)
            {
                board[pawn.position_y + 1][pawn.position_x + 1] = &pawn;//nowa pozycja
                board[pawn.position_y][pawn.position_x] = nullptr;//stara pozcyja
                pawn.position_y = pawn.position_y + 1;
                pawn.position_x = pawn.position_x + 1;
            }
        }

        void move_right_backwards(Pawn &pawn)
        {
            if(!pawn.team && pawn.actions.RIGHT_MOVE_BACKWARD)
            {
                board[pawn.position_y + 1][pawn.position_x + 1] = &pawn; // nowa pozycja
                board[pawn.position_y][pawn.position_x] = nullptr; // stara pozycja
                pawn.position_y = pawn.position_y  + 1;
                pawn.position_x = pawn.position_x + 1;
            }
            else if(pawn.team && pawn.actions.RIGHT_MOVE_BACKWARD)
            {
                board[pawn.position_y - 1][pawn.position_x + 1] = &pawn; // nowa pozycja
                board[pawn.position_y][pawn.position_x] = nullptr; // stara pozycja
                pawn.position_y = pawn.position_y  - 1;
                pawn.position_x = pawn.position_x + 1;
            }
        }

        void strike_left(Pawn &pawn)
        {
            if(!pawn.team && pawn.actions.LEFT_STRIKE_FORWARD)
            {
                board[pawn.position_y - 2][pawn.position_x - 2] = &pawn;//nowa pozycja
                board[pawn.position_y - 1][pawn.position_x - 1]->life_death = false;
                board[pawn.position_y - 1][pawn.position_x - 1] = nullptr;//pozycja bita
                board[pawn.position_y][pawn.position_x] = nullptr;//stara pozcyja 
                pawn.position_y = pawn.position_y - 2;
                pawn.position_x = pawn.position_x - 2;
            }
            else if(pawn.team && pawn.actions.LEFT_STRIKE_FORWARD)
            {
                board[pawn.position_y + 2][pawn.position_x - 2] = &pawn;//nowa pozycja
                board[pawn.position_y + 1][pawn.position_x - 1]->life_death = false;
                board[pawn.position_y + 1][pawn.position_x - 1] = nullptr;//pozycja bita
                board[pawn.position_y][pawn.position_x] = nullptr;//stara pozcyja  
                pawn.position_y = pawn.position_y + 2;
                pawn.position_x = pawn.position_x - 2;
            }
        }

        void strike_left_backwards(Pawn &pawn)
        {
            if(!pawn.team && pawn.actions.LEFT_STRIKE_BACKWARD)
            {
                board[pawn.position_y + 2][pawn.position_x - 2] = &pawn; // nowa pozycja
                board[pawn.position_y + 1][pawn.position_x - 1]->life_death = false;
                board[pawn.position_y + 1][pawn.position_x - 1] = nullptr; // pozycja bita
                board[pawn.position_y][pawn.position_x] = nullptr; // stara pozycja
                pawn.position_y = pawn.position_y + 2;
                pawn.position_x = pawn.position_x - 2;
            }
        else if(pawn.team && pawn.actions.LEFT_STRIKE_BACKWARD)
            {
                board[pawn.position_y - 2][pawn.position_x - 2] = &pawn; // nowa pozycja
                board[pawn.position_y - 1][pawn.position_x - 1]->life_death = false;
                board[pawn.position_y - 1][pawn.position_x - 1] = nullptr; // pozycja bita
                board[pawn.position_y][pawn.position_x] = nullptr; // stara pozycja
                pawn.position_y = pawn.position_y - 2;
                pawn.position_x = pawn.position_x - 2;
            }
        }

        void strike_right(Pawn &pawn)
        {
            if(!pawn.team && pawn.actions.RIGHT_STRIKE_FORWARD)
            {
                board[pawn.position_y - 2][pawn.position_x + 2] = &pawn;//nowa pozycja
                board[pawn.position_y - 1][pawn.position_x + 1]->life_death = false;
                board[pawn.position_y - 1][pawn.position_x + 1] = nullptr;//pozycja bita
                board[pawn.position_y][pawn.position_x] = nullptr;//stara pozcyja
                pawn.position_y = pawn.position_y - 2;
                pawn.position_x = pawn.position_x + 2;
                
            }
            else if(pawn.team && pawn.actions.RIGHT_STRIKE_FORWARD)
            {
                board[pawn.position_y + 2][pawn.position_x + 2] = &pawn;//nowa pozycja
                board[pawn.position_y + 1][pawn.position_x + 1]->life_death = false;
                board[pawn.position_y + 1][pawn.position_x + 1] = nullptr;//pozycja bita
                board[pawn.position_y][pawn.position_x] = nullptr;//stara pozcyja
                pawn.position_y = pawn.position_y + 2;
                pawn.position_x = pawn.position_x + 2;
            }
        }

        void strike_right_backwards(Pawn &pawn)
        {
            if(!pawn.team && pawn.actions.RIGHT_STRIKE_BACKWARD)
            {
                board[pawn.position_y + 2][pawn.position_x + 2] = &pawn; // nowa pozycja
                board[pawn.position_y + 1][pawn.position_x + 1]->life_death = false;
                board[pawn.position_y + 1][pawn.position_x + 1] = nullptr; // pozycja bita
                board[pawn.position_y][pawn.position_x] = nullptr; // stara pozycja
                pawn.position_y = pawn.position_y + 2;
                pawn.position_x = pawn.position_x + 2;
            }
        else if(pawn.team && pawn.actions.RIGHT_STRIKE_BACKWARD)
            {
                board[pawn.position_y - 2][pawn.position_x + 2] = &pawn; // nowa pozycja
                board[pawn.position_y - 1][pawn.position_x + 1]->life_death = false;
                board[pawn.position_y - 1][pawn.position_x + 1] = nullptr; // pozycja bita
                board[pawn.position_y][pawn.position_x] = nullptr; // stara pozycja
                pawn.position_y = pawn.position_y - 2;
                pawn.position_x = pawn.position_x + 2;
            }
        }        

        //this function make you move and stirke
        void move_man(Pawn &pawn, USER_DECISION decision)//true - left, false - right
        {
            chec_action(pawn);
            switch (decision)
            {
            case USER_DECISION::LEFT_MOVE_FORWARD :
            {
                move_left(pawn);
                break;
            }
            case USER_DECISION::LEFT_MOVE_BACKWARD :
            {
                move_left_backwards(pawn);
                break;
            }
            case USER_DECISION::RIGHT_MOVE_FORWARD :
            {
                move_rigth(pawn);
                break;
            }
            case USER_DECISION::RIGHT_MOVE_BACKWARD :
            {
                move_right_backwards(pawn);
                break;
            }
            case USER_DECISION::LEFT_STRIKE_FORWARD :
            {
                strike_left(pawn);
                break;
            }
            case USER_DECISION::LEFT_STRIKE_BACKWARD :
            {
                strike_left_backwards(pawn);
                break;
            }
            case USER_DECISION::RIGHT_STRIKE_FORWARD :
            {
                strike_right(pawn);
                break;
            }
            case USER_DECISION::RIGHT_STRIKE_BACKWARD :
            {
                strike_right_backwards(pawn);
                break;
            }
            }
        }

        void print_board()
        {
            for (int i = 0; i < ROWS; i++)
            {
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

        //tutaj ta funckja nie jest potrzeban mozesz tutja ustawic w kalsie tylkozmienne poszczegolnych 
        //druzy i tylko za pomoca tej funckji sprawdzac ich wartossc
        bool game_over()
        {
            int white_points = 0;
            int black_points = 0;
            for(int i = 0; i < pawns.size() ; i++)
            {
                if(pawns[i].team && pawns[i].life_death == false)
                {
                    white_points++;
                }
                else if(!pawns[i].team && pawns[i].life_death == false)
                {
                    black_points++;
                }
            }

            if(white_points == 12 || black_points == 12)
            {
                return true;
            }
            return false;
        }

        
        int evaluate_board(bool team)
        {
            int white_points = 0;
            int black_points = 0;
            for(int i = 0; i < pawns.size() ; i++)
            {
                if(pawns[i].team && pawns[i].life_death == false)
                {
                    white_points++;
                }
                else if(!pawns[i].team && pawns[i].life_death == false)
                {
                    black_points++;
                }
            }

            if(team)
            {
                return white_points;
            }
            else
            {
                return black_points;
            }
        }

        //zwraca wektor z id wszystkich pionkow ktroe faktycznie moga sie ruszyc
        std::vector<int> generete_moves()
        {
            std::vector<int> pawns_with_moves;
            for(int i = 0; i < pawns.size() ; i++)
            {
                if(pawns[i].team)
                {
                    chec_action(pawns[i]);
                    if(pawns[i].actions.any_true())
                    {
                        pawns_with_moves.push_back(i);
                    }
                }
                else
                {
                    chec_action(pawns[i]);
                    if(pawns[i].actions.any_true())
                    {
                        pawns_with_moves.push_back(i);
                    }
                }
                
            }
            return pawns_with_moves;
        }

        //zwraca pare ktora mowi co trzeba zrobic oraz dla jakiego id bo kurwa nie umiem tam PAWN dac japierdole
        std::pair<USER_DECISION,int> the_best_move(std::vector<int> pawns_id)
        {
            int old_best_move = 0;
            for(int i = 0; i < pawns_id.size(); i++)
            {
                int new_best_move = 0;
                if(pawns[pawns_id[i]].actions.move_true())
                {
                    new_best_move = 1;
                }
                
                while(pawns[pawns_id[i]].actions.strike_true())
                {
                    new_best_move = 5;
                    //kontynuuj
                }

                if (old_best_move < new_best_move )
                {
                    old_best_move = new_best_move;
                }
            }
        }
};


int main()
{
    Board board;
    board.print_board();
    Pawn &pawn_white = board.pawns[9];
    Pawn &pawn_black = board.pawns[12];
    pawn_black.man_king = true;
    board.move_man(pawn_white,board.LEFT_MOVE_FORWARD);
    board.move_man(pawn_white,board.LEFT_MOVE_FORWARD);
    std::cout << std::endl;
    board.print_board();
    board.move_man(pawn_black,board.RIGHT_STRIKE_FORWARD);
    std::cout << std::endl;
    board.print_board();
    std::cout << std::endl;
    std::cout << pawn_white.life_death << std::endl;


    
    // Pawn &pawn = board.pawns[12];
    // std::cout << pawn.position_x << " " << pawn.position_y << " " << pawn.team << std::endl;
    // board.chec_action(pawn);

    // std::cout << "LEFT_MOVE_FORWARD: " << (pawn.actions.LEFT_MOVE_FORWARD ? "true" : "false") << std::endl;
    // std::cout << "RIGHT_MOVE_FORWARD: " << (pawn.actions.RIGHT_MOVE_FORWARD ? "true" : "false") << std::endl;
    // std::cout << "LEFT_MOVE_BACKWARD: " << (pawn.actions.LEFT_MOVE_BACKWARD ? "true" : "false") << std::endl;
    // std::cout << "RIGHT_MOVE_BACKWARD: " << (pawn.actions.RIGHT_MOVE_BACKWARD ? "true" : "false") << std::endl;
    // std::cout << "LEFT_STRIKE_FORWARD: " << (pawn.actions.LEFT_STRIKE_FORWARD ? "true" : "false") << std::endl;
    // std::cout << "RIGHT_STRIKE_FORWARD: " << (pawn.actions.RIGHT_STRIKE_FORWARD ? "true" : "false") << std::endl;
    // std::cout << "LEFT_STRIKE_BACKWARD: " << (pawn.actions.LEFT_STRIKE_BACKWARD ? "true" : "false") << std::endl;
    // std::cout << "RIGHT_STRIKE_BACKWARD: " << (pawn.actions.RIGHT_STRIKE_BACKWARD ? "true" : "false") << std::endl;

    return 0;
}




