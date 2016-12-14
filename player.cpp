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

int Player::getUnit(std::string location, Unit* unit)
{
    for (size_t i = 0; i < units_.size(); ++i) {
        if (units_[i].location_->getName() == location) {
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

void Player::moveUnit(std::string start, Province* dest)
{
    Unit unit;
    getUnit(start, &unit);
    
    unit.location_ = dest;

    if (dest->hasCenter()) {
        supplyCenters_.push_back((LandProvince*)dest);
    }
}

void Player::removeCenter(LandProvince* province)
{
    for (auto it = supplyCenters_.begin(); it != supplyCenters_.end(); ++it) {
        if ((*it)->getName() == province->getName()) {
            supplyCenters_.erase(it);
            return;
        }
    }
}

void Player::placeUnit(LandProvince* prov, std::string type, std::string coastName)
{
    Unit unit;
    unit.location_ = prov;

    if (type == "fleet") {
        unit.type_ = Player::Unit::fleet;

        Coast* coast = prov->getCoast(coastName);

        unit.coast_ = coast;
    } else {
        unit.type_ = Player::Unit::army;
    }

    units_.push_back(unit);
}

size_t Player::getNumSupplyCenters()
{
    return supplyCenters_.size();
}
