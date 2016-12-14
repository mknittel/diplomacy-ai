/**
 * board.hpp
 * 
 * Interface. Keeps track of the board set up
 * and its current state.
 */

#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED = 1;

#include <string>
#include <vector>
#include <iostream>

#include "player.hpp"
#include "province.hpp"
#include "landprovince.hpp"
#include "seaprovince.hpp"
#include "moveparser.hpp"

class Board {
public:
    /**
     * \brief Creates a board with no countries or
     * players (except a neutral player).
     */
    Board();

    /**
     * \brief Creates a board with the given provinces
     * and players.
     */
    Board(std::vector<Province> provinces, std::vector<Player> players);

    /**
     * \brief Uploads a board setup from a file.
     */
    void uploadBoard(std::string filename);

    /**
     * \brief Uploads and executes a Spring/Fall move from a file.
     */
    std::vector<Player::Retreat> executeMoves(std::string filename);

    /**
     * \brief Uploads and executes retreats from a file.
     */
    void executeRetreats(std::string filename);

    /**
     * \brief Uploads and executes builds and destructions from a file.
     */
    void executeBuilds(std::string filename);

    /**
     * \brief Returns a list of the players.
     */
    std::vector<Player*> getPlayers();

    /**
     * \brief Returns the number of centers controlled by player.
     */
    size_t getNumCenters();

private:
    std::vector<LandProvince> landProvinces_;
    std::vector<SeaProvince> seaProvinces_;
    std::vector<Player> players_;
    std::vector<Coast> coasts_;
    
    // Helper for uploadBoards, updates neighbors of land provinces
    //void updateLandNeighbors();

    // Helper for uploadBoards, retrieves a pointer to province if exists
    int getLandProvince(std::string name);

    // Helper for uploadBoards, retrieves a pointer to province if exists
    int getSeaProvince(std::string name);

    // Helper for executeMoves, retrieves a pointer to province if exists
    int getProvince(std::string name);

    // Helper for executeMoves, retrieves a pointer to the player if exists
    int getPlayer(std::string name);

    // Helper for uploadBoards, reads file
    void interpretFile(std::string filename,
                       std::vector<std::vector<std::string>>* landProvinces,
                       std::vector<std::vector<std::string>>* seaProvinces,
                       std::vector<std::vector<std::string>>* coastStrings,
                       std::vector<std::vector<std::string>>* playerStrings,
                       std::vector<std::string>*              centerStrings);

    // Helper for uploadBoards, links coasts
    void linkCoasts(std::vector<std::vector<std::string>> coastStrings);

    // Helper for uploadBoards, links land
    void linkLand(std::vector<std::vector<std::string>> landProvinces);

    // Helper for uploadBoards, links seas
    void linkSeas(std::vector<std::vector<std::string>> seaProvinces);

    // Helper for uploadBoards, gives players home centers
    void setHomes(std::vector<std::vector<std::string>> playerStrings);

    // Helper for uploadBoards, makes neutral centers
    void addCenters(std::vector<std::string> centerStrings);

    // Helper for executeMoves, calls the execute modifies players and provinces
    void modifyBoard(std::string start, std::string dest, std::string playerName);
};

#endif // ifndef BOARD_HPP_INCLUDED
