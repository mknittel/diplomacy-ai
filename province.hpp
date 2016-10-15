/**
 * province.hpp
 *
 * Interface. Defines a province on the board.
 * Contains information on its name, if it has
 * a supply center, who controls it, which
 * coasts it has, and which neighbors it has.
 */

#ifndef PROVINCE_HPP_INCLUDED
#define PROVINCE_HPP_INCLUDED

#include <string>
#include "player.hpp"

class Province {
public:
    /**
     * /brief Defines a province with a given name, that may
     * or may not have a supply center, under a player's 
     * control (or neutral), with some coasts, and some
     * neighboring provinces.
     * 
     * /post The controller only matters if this province
     * has a supply center.
     */
    Province(std::string name, bool hasCenter, Player controller, int coasts[]);

private:
    std::string name_;
    bool hasCenter_;
    Player controller_;
    int* coasts_;
    Province** neighbors_;
    int numNeighbors_;
};

#endif // ifndef PROVINCE_HPP_INCLUDED
