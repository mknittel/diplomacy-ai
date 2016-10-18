/**
 * player.cpp
 *
 * Implementation. Defines a player who can play the game.
 */

#include <string>
#include <iostream>

#include "player.hpp"

Player::Player(std::string name): name_(name) { }

void Player::addSupplyCenter(LandProvince* landProvince)
{
    supplyCenters_.push_back(landProvince);
}

void Player::removeSupplyCenter(LandProvince* landProvince)
{
    for (size_t i = 0; i < supplyCenters_.size(); ++i) {
        if (landProvince->getName() == supplyCenters_[i]->getName()) {
            supplyCenters_.erase(supplyCenters_.begin() + i);
        }
    }
}
