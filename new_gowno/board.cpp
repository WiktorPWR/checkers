#include <vector>



class Board
{
    private:
        const int ROWS = 8;
        const int COLUMNS = 8;
        std::vector<std::vector<Pawn*>> board;
        enum MoveDirection
        { 
            LEFT_MOVE,
            RIGHT_MOVE,
            BOTH_MOVE,
            LEFT_STRIKE,
            RIGHT_STRIKE,
            LEFT_STRIKE,
            RIGHT_STRIKE,
            BOTH_STRIKE,
            NONE,
        };
        
    public:

        Board()
        {
            for (int i = 0; i < ROWS; i++)
            {
                std::vector<Pawn*> ROW;
                for (int j = 0; j < COLUMNS; j++)
                {
                    if (i < 3)
                    {
                        if ((i + j) % 2 != 0)
                        {
                            Pawn pawn_white;
                            pawn_white.position_y = i;
                            pawn_white.position_x = j;
                            pawn_white.team = true;
                            ROW.push_back(&pawn_white);
                        }
                    }
                    else if (i >= ROWS - 3)
                    {
                        if ((i + j) % 2 != 0)
                        {
                            Pawn pawn_black;
                            pawn_black.position_y = j;
                            pawn_black.position_x = i;
                            pawn_black.team = true;
                            ROW.push_back(&pawn_black);
                        }
                    }
                    else
                    {
                        ROW.push_back(nullptr);
                    }
                }
            }
        }

        bool chec_left(Pawn &pawn)
        {
            if(board[pawn.position_y - 1][pawn.position_x - 1] == nullptr && pawn.team)//bialasy
            {
                return true;
            }
            else if(board[pawn.position_y + 1][pawn.position_x - 1] == nullptr && !pawn.team)
            {
                return true;
            }
            else 
            {
                return false;
            }

        }

        bool chec_right(Pawn &pawn)
        {
            if(board[pawn.position_y - 1][pawn.position_x + 1] == nullptr && pawn.team)//bialasy
            {
                return true;
            }
            else if(board[pawn.position_y + 1][pawn.position_x + 1] == nullptr && !pawn.team)
            {
                return true;
            }
            else 
            {
                return false;
            }
        }
       
        bool chec_strike_left(Pawn &pawn)
        {
            //bialasy
            if(board[pawn.position_x - 1][pawn.position_y - 1]->team != pawn.team && pawn.team)
            {
                if(board[pawn.position_x - 2][pawn.position_y - 2] == nullptr)
                {
                    //bij czarnucha
                    return true;
                }
            }
            //czarnuchy
            else if(board[pawn.position_x + 1][pawn.position_y - 1]->team != pawn.team && pawn.team) 
            {
                if(board[pawn.position_x + 2][pawn.position_y - 2] == nullptr)
                {
                    //bij bialy
                    return true;
                }
            }
            else
            {
                return false;
            }
        }

        bool chec_strike_right(Pawn &pawn)
        {
            //bialasy
            if(board[pawn.position_x - 1][pawn.position_y + 1]->team != pawn.team && pawn.team)
            {
                if(board[pawn.position_x - 2][pawn.position_y + 2] == nullptr)
                {
                    //bij czarnucha
                    return true;
                }
            }
            //czarnuchy
            else if(board[pawn.position_x + 1][pawn.position_y + 1]->team != pawn.team && pawn.team) 
            {
                if(board[pawn.position_x + 2][pawn.position_y + 2] == nullptr)
                {
                    //bij bialy
                    return true;
                }
            }
            else
            {
                return false;
            }
        }
        //sprawdzenie czy mozesz sie ruszyc
        MoveDirection chec_action(Pawn &pawn)
        {

            if(pawn.team)//bialasy
            {
                if(chec_strike_left(pawn) && chec_strike_right(pawn))
                {
                    return MoveDirection::BOTH_STRIKE;
                }
                else if(chec_strike_left(pawn))
                {
                    return MoveDirection::LEFT_STRIKE;
                }
                else if(chec_strike_right(pawn))
                {
                    return MoveDirection::RIGHT_STRIKE;
                }
                else if(chec_left(pawn) && chec_right(pawn))
                {
                    return MoveDirection::BOTH_MOVE;
                }
                else if(chec_left(pawn))
                {
                    return MoveDirection::LEFT_MOVE;
                }
                else if(chec_right(pawn))
                {
                    return MoveDirection::RIGHT_MOVE;
                }
                else
                {
                    return MoveDirection::NONE;
                }
            }
            else //czarnuchy
            {
                if(chec_strike_left(pawn) && chec_strike_right(pawn))
                {
                    return MoveDirection::BOTH_STRIKE;
                }
                else if(chec_strike_left(pawn))
                {
                    return MoveDirection::LEFT_STRIKE;
                }
                else if(chec_strike_right(pawn))
                {
                    return MoveDirection::RIGHT_STRIKE;
                }
                else if(chec_left(pawn) && chec_right(pawn))
                {
                    return MoveDirection::BOTH_MOVE;
                }
                else if(chec_left(pawn))
                {
                    return MoveDirection::LEFT_MOVE;
                }
                else if(chec_right(pawn))
                {
                    return MoveDirection::RIGHT_MOVE;
                }
                else
                {
                    return MoveDirection::NONE;
                }
            }
        }

        void move_left(Pawn &pawn)
        {
            if(pawn.team)
            {
            board[pawn.position_y - 1][pawn.position_x - 1] = &pawn;//nowa pozycja
            board[pawn.position_y][pawn.position_x] = nullptr;//stara pozcyja
            }
            else
            {
                board[pawn.position_y + 1][pawn.position_x - 1] = &pawn;//nowa pozycja
                board[pawn.position_y][pawn.position_x] = nullptr;//stara pozcyja
            }
        }

        void move_rigth(Pawn &pawn)
        {
            if(pawn.team)
            {
            board[pawn.position_y - 1][pawn.position_x + 1] = &pawn;//nowa pozycja
            board[pawn.position_y][pawn.position_x] = nullptr;//stara pozcyja
            }
            else
            {
                board[pawn.position_y + 1][pawn.position_x + 1] = &pawn;//nowa pozycja
                board[pawn.position_y][pawn.position_x] = nullptr;//stara pozcyja
            }
        }

        void strike_left(Pawn &pawn)
        {
            if(pawn.team)
            {
                board[pawn.position_y - 2][pawn.position_x - 2] = &pawn;//nowa pozycja
                board[pawn.position_y - 1][pawn.position_x - 1] = nullptr;//pozycja bita
                board[pawn.position_y][pawn.position_x] = nullptr;//stara pozcyja   
            }
            else
            {
                board[pawn.position_y + 2][pawn.position_x - 2] = &pawn;//nowa pozycja
                board[pawn.position_y + 1][pawn.position_x - 1] = nullptr;//pozycja bita
                board[pawn.position_y][pawn.position_x] = nullptr;//stara pozcyja  
            }
        }

        void strike_right(Pawn &pawn)
        {
            if(pawn.team)
            {
                board[pawn.position_y - 2][pawn.position_x + 2] = &pawn;//nowa pozycja
                board[pawn.position_y - 1][pawn.position_x + 1] = nullptr;//pozycja bita
                board[pawn.position_y][pawn.position_x] = nullptr;//stara pozcyja
            }
            else
            {
                board[pawn.position_y + 2][pawn.position_x + 2] = &pawn;//nowa pozycja
                board[pawn.position_y + 1][pawn.position_x + 1] = nullptr;//pozycja bita
                board[pawn.position_y][pawn.position_x] = nullptr;//stara pozcyja
            }
        }

        void move_pawn(MoveDirection ENUM,Pawn &pawn, bool decision_left_right)//true - left, false - right
        {
            switch (ENUM)
            {
            case MoveDirection::LEFT_STRIKE :
                {
                    strike_left(pawn);
                    break;
                }
            
            case MoveDirection::RIGHT_STRIKE :
                {
                    strike_right(pawn);
                    break;
                }

            case MoveDirection::BOTH_STRIKE :
                {
                    if(decision_left_right)
                    {
                        strike_left(pawn);
                    } 
                    else
                    {
                        strike_right(pawn);
                    }
                    //wybor
                    break;
                }

            case MoveDirection::LEFT_MOVE :
                {
                    move_left(pawn);
                    break;  
                }
            
            case MoveDirection::RIGHT_MOVE :
                {
                    move_rigth(pawn);
                    break;
                }

            case MoveDirection::BOTH_MOVE :
                {
                    if(decision_left_right)
                    {
                        move_left(pawn);
                    }
                    else
                    {
                        move_rigth(pawn);
                    }
                    break;
                }

            case MoveDirection::NONE :
                break;
            }
        }

        
};


class Pawn
{
    public:
        bool team;// 1 - whitem 0 black
        int position_x;// column
        int position_y;// row
        
};





