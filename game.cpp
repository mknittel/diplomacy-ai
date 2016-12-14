/**
 * game.cpp
 * 
 * Implementation. Runs the game as series of moves.
 * Also deals with board initiation.
 */

#include "game.hpp"

Game::Game(std::string boardfile)
{
    std::cout<<"preboard\n";
    board_.uploadBoard(boardfile);
    numCenters_ = board_.getNumCenters();
    runGame();
}

void Game::runGame()
{
    Player* player;

    while (true) {
        // Spring round
        getMoves();

        if (checkWin(player)) {
            return;
        }

        // Fall round
        getMoves();
    
        if (checkWin(player)) {
            return;
        }

        // Winter round
        buildSeason();
    }
}

void Game::getMoves()
{
    std::cout << "Please enter the name of your move file.\n";

    std::string orderfile;
    std::cin >> orderfile;

    std::vector<Player::Retreat> retreats = board_.executeMoves(orderfile);    
    getRetreats(retreats);
}

void Game::getRetreats(std::vector<Player::Retreat> retreats)
{
    std::cout << "The retreats that must be made are:\n";

    for (size_t i = 0; i < retreats.size(); ++i) {
        std::cout << "Player Name: " << retreats[i].player_ << ", Province: "
                  << retreats[i].prov_ << ".\n";
    }

    std::cout << "Please enter the name of your retreat file.\n";

    std::string retreatfile;
    std::cin >> retreatfile;

    board_.executeRetreats(retreatfile);
}

void Game::buildSeason()
{
    std::cout << "Please enter the name of your build file.\n";
    
    std::string buildfile;
    std::cin >> buildfile;

    board_.executeBuilds(buildfile);
}

bool Game::checkWin(Player* player)
{
    std::vector<Player*> players = board_.getPlayers();

    for (size_t i = 0; i < players.size(); ++i) {
        Player* nextPlayer = players[i];
        size_t numCenters = nextPlayer->getNumSupplyCenters();

        if ((float) numCenters / numCenters_ > .529) {
            player = nextPlayer;
            return true;
        } 
    }
    
    return false;
}
