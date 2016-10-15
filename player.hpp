/**
 * player.hpp
 *
 * Interface. Defines a player who can play the game.
 */

#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <string>

class Player {
public:
    /**
     * /brief Creates a player with the given
     * name.
     */
    Player(std::string name);

private:
    std::string name_;
};

#endif // ifndef PLAYER_HPP_INCLUDED
