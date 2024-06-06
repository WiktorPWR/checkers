#include "game.h"
#include "board.h"
#include <iostream>
#include <limits>
#include "minimax.h"


void play_game()
{
    board_init();
    std::cout << "Plansza:" << std::endl;
    print_board();

    bool team = true; // true for white, false for black

    while(white_pawns != 0 && black_pawns != 0 && !game_over())
    {
        bool valid_action = false;
        if (team) // Player's turn (white)
        {
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
                bool L_R, back;

                switch (action)
                {
                case 1:
                    if (has_captures_bool(team))
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
                        if (result.can_move_back_left)
                            std::cout << "3. Ruch w tył w lewo" << std::endl;
                        if (result.can_move_back_right)
                            std::cout << "4. Ruch w tył w prawo" << std::endl;
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
                                move_pawn(row, column, team, result, false, false);
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
                                move_pawn(row, column, team, result, true, false);
                                valid_action = true;
                            }
                            else
                            {
                                std::cout << "Nieprawidłowa akcja. Spróbuj ponownie." << std::endl;
                            }
                            break;
                        case 3:
                            if (result.can_move_back_left)
                            {
                                move_pawn(row, column, team, result, false, true);
                                valid_action = true;
                            }
                            else
                            {
                                std::cout << "Nieprawidłowa akcja. Spróbuj ponownie." << std::endl;
                            }
                            break;
                        case 4:
                            if (result.can_move_back_right)
                            {
                                move_pawn(row, column, team, result, true, true);
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
                    if (result.can_capture_back_left)
                        std::cout << "3. Bicie w tył w lewo" << std::endl;
                    if (result.can_capture_back_right)
                        std::cout << "4. Bicie w tył w prawo" << std::endl;
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
                            strike_pawn(row, column, team, result, false, false);
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
                            strike_pawn(row, column, team, result, true, false);
                            valid_action = true;
                        }
                        else
                        {
                            std::cout << "Nieprawidłowa akcja. Spróbuj ponownie." << std::endl;
                        }
                        break;
                    case 3:
                        if (result.can_capture_back_left)
                        {
                            strike_pawn(row, column, team, result, false, true);
                            valid_action = true;
                        }
                        else
                        {
                            std::cout << "Nieprawidłowa akcja. Spróbuj ponownie." << std::endl;
                        }
                        break;
                    case 4:
                        if (result.can_capture_back_right)
                        {
                            strike_pawn(row, column, team, result, true, true);
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
        }
        // else // AI's turn (black)
        // {
        //     //best_solution_max_player();
        //     valid_action = true;
        // }

        // Switch teams for next turn
        team = !team;
        print_board();
    }

    std::cout << "Gra zakończona! " << (white_pawns == 0 ? "Czarne" : "Białe") << " wygrywają!" << std::endl;
}
