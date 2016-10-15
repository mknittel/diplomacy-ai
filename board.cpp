/**
 * board.cpp
 * 
 * Implementation. Keeps track of the board set up
 * and its current state.
 */

#include <iostream>
#include "board.hpp"

using namespace std;

Board::Board(int provinces[], Player players[])
: provinces_(provinces),
  players_(players)
{

};
