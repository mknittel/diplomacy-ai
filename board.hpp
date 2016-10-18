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

private:
    std::vector<LandProvince> landProvinces_;
    std::vector<SeaProvince> seaProvinces_;
    std::vector<Player> players_;


    // Helper for uploadBoards, updates neighbors of land provinces
    void updateLandNeighbors();

    // Helper for uploadBoards, retrieves a pointer to province if exists
    int getLandProvince(std::string name, LandProvince* province);

    // Helper for uploadBoards, retrieves a pointer to province if exists
    int getSeaProvince(std::string name, SeaProvince* province);

    // Helper for uploadBoards, reads file
    void interpretFile(std::string filename,
                       std::vector<std::vector<std::string>>* landProvinces,
                       std::vector<std::vector<std::string>>* seaProvinces,
                       std::vector<std::vector<std::string>>* coastStrings,
                       std::vector<std::vector<std::string>>* playerStrings,
                       std::vector<std::string>*              centerStrings);

    // Helper for uploadBoards, links coasts
    void linkCoasts(std::vector<std::vector<std::string>> coastStrings,
                    std::vector<Coast>* coasts);

    // Helper for uploadBoards, links land
    void linkLand(std::vector<std::vector<std::string>> landProvinces);

    // Helper for uploadBoards, links seas
    void linkSeas(std::vector<std::vector<std::string>> seaProvinces);

    // Helper for uploadBoards, makes neutral centers
    void addCenters(std::vector<std::string> centerStrings);
};

#endif // ifndef BOARD_HPP_INCLUDED
