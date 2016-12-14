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
     * \struct Unit 
     * 
     * \brief Represents a given unit owned by the player,
     * where it is, and what type it is.
     */
    struct Unit {
        enum Type { fleet, army };

        Type type_;
        Province* location_;
        Coast* coast_;
    };

    struct Hold {
        std::string prov_;
        std::string player_;
    };

    struct Convoy {
        std::string dest_;
        std::string start_;
        std::vector<std::string> convoy_;
        std::string player_;
    };

    struct Move {
        std::string dest_;
        std::string start_;
        std::string player_;
    };

    struct Support {
        std::string prov_;
        std::string player_;
    };

    struct SupportHold : Support {
        Hold* hold_;
    };

    struct SupportConvoy : Support {
        Convoy* convoy_;
    };

    struct SupportMove : Support {
        Move* move_;
    };
    
    struct Retreat {
        std::string prov_;
        std::string retreat_;
        std::string player_;
    };

    struct Build {
        std::string prov_;
        std::string player_;
        std::string type_;

        std::string coast_;
    };

    struct MoveSet {
        std::vector<Hold> holds_;
        std::vector<Convoy> convoys_;
        std::vector<Move> moves_;
        std::vector<SupportHold> supportHolds_;
        std::vector<SupportConvoy> supportConvoys_;
        std::vector<SupportMove> supportMoves_;
        std::vector<Retreat> retreats_;
    };

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
     * \brief Returns player name.
     */
    std::string getName();

    /**
     * \brief Returns a pointer to the unit at the
     * given location.
     */
    int getUnit(std::string location, Unit* unit);

    /**
     * \breif Adds a province to the home land of the player.
     */
    void addHomeCenter(LandProvince* landProvince);

    /**
     * \brief Executes a move given the start and destination.
     */
    void moveUnit(std::string start, Province* dest);

    /**
     * \brief Removes province from control.
     */
    void removeCenter(LandProvince* province);

    /**
     * \brief Places a unit of a type at a location.
     */
    void placeUnit(LandProvince* prov, std::string type, std::string coast);
    
    /**
     * \brief Returns the number of supply centers owned by player.
     */
    size_t getNumSupplyCenters();

private:
    std::string name_;
    std::vector<LandProvince*> supplyCenters_;
    std::vector<Unit> units_;
    std::vector<LandProvince*> home_;
};

#endif // ifndef PLAYER_HPP_INCLUDED
