/**
 * board.hpp
 * 
 * Interface. Keeps track of the board set up
 * and its current state.
 */

#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED = 1;

#include <string>
#include "player.hpp"
#include "province.hpp"

class Board {
public:
    /**
     * /brief Creates a board with no countries
     */
    Board();

    /**
     * /brief Creates a board with the given provinces
     * and players.
     */
    Board(int provinces[], Player players[]);

private:
    int* provinces_;
    Player* players_;
};

#endif // ifndef BOARD_HPP_INCLUDED
