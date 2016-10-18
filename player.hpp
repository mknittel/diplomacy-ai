/**
 * player.hpp
 *
 * Interface. Defines a player who can play the game.
 */

#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <string>
#include "landprovince.hpp"
#include "seaprovince.hpp"

class Player {
public:
    /**
     * \brief Creates a player with the given
     * name and zero supply centers.
     */
    Player(std::string name);

    // Delete default constructor
    Player() = delete;

    /**
     * \brief Gives player a supply center.
     */
    void addSupplyCenter(LandProvince* landProvince);

    /**
     * \brief Removes a supply center from a player.
     */
    void removeSupplyCenter(LandProvince* landProvince);

    /**
     * \struct Unit 
     * 
     * \brief Represents a given unit owned by the player,
     * where it is, and what type it is.
     */
    struct Unit {
        enum Type { fleet, army};

        Type type;
        Province* location;
    };

private:
    std::string name_;
    std::vector<LandProvince*> supplyCenters_;
    std::vector<Unit> units_;
};

#endif // ifndef PLAYER_HPP_INCLUDED
