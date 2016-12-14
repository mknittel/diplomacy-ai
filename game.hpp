/**
 * game.hpp
 * 
 * Interface. Runs the game as series of moves.
 * Also deals with board initiation.
 */

#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED = 1;

#include <string>
#include <vector>
#include <iostream>

#include "board.hpp"

class Game {
public:
    /**
     * \brief Constructor. Sets up board.
     */
    Game(std::string boardfile);

    /**
     * \brief Runs the game. Calls all functions
     * for game rounds in sequence. Prompts user
     * for file input.
     */
    void runGame();
    
private:
    Board board_;
    size_t numCenters_;

    // Gets and executes Spring/Fall moves
    void getMoves();

    // Gets and executes Spring/Fall retreats
    void getRetreats(std::vector<Player::Retreat> retreats);
    
    // Gets and executes builds and destroys
    void buildSeason();

    // Returns true and gives the player if someone has won
    bool checkWin(Player* player);
};

#endif // ifndef GAME_HPP_INCLUDED
