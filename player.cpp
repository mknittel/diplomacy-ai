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

std::string Player::getName()
{
    return name_;
}

int Player::getUnit(Player::Unit::Type type, std::string location, Unit* unit)
{
    for (size_t i = 0; i < units_.size(); ++i) {
        if (units_[i].type_ == type && units_[i].location_->getName() == location) {
            unit = &units_[i]; 
            return 1;
        }
    }

    return 0;
}

void Player::addHomeCenter(LandProvince* landProvince)
{
    home_.push_back(landProvince);
}
