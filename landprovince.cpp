/**
 * landprovince.hpp
 *
 * Implementation. Defines a land province on the board.
 * Contains information on its name, if it has
 * a supply center, who controls it, which
 * coasts it has, and which neighbors it has.
 */

#include <string>
#include <vector>
#include <iostream>
#include "player.hpp"
#include "landprovince.hpp"
#include "coast.hpp"

void LandProvince::addCoast(Coast* coast, std::vector<Province*> neighbors)
{
    coasts_.push_back(coast);

    for (size_t i = 0; i < neighbors.size(); ++i) {
        Province* province = neighbors[i];
        
        if (!hasNeighbor(province)) {
            addNeighbor(province);
        }
    }
}

/*void LandProvince::updateNeighbors()
{
    for (size_t i = 0; i < coasts_.size(); ++i) {
        Coast* coast = coasts_[i];
        std::vector<Coast*> connections(coast->getConnections());

        for (size_t j = 0; j < connections.size(); ++j) {
            LandProvince* province(connections[j]->getProvince());

            if (!hasNeighbor(province)) {
                addNeighbor(province);
            }
        }
    }
}*/

std::vector<Coast*> LandProvince::getCoasts()
{
    return coasts_;
}

Coast* LandProvince::getCoast(std::string name)
{
    for (size_t i = 0; i < coasts_.size(); ++i) {
        std::cout<<"Found coast: " << coasts_[i]->getName()<<"\n";
        if (coasts_[i]->getName() == name) {
            return coasts_[i];
        }
    }

    return nullptr;
}

void LandProvince::makeCenter()
{
    isCenter_ = true;
}

void LandProvince::printCoasts()
{
    std::cout << "Coasts for " << getName() << ": ";

    for (size_t i = 0; i < coasts_.size(); ++i) {
        std::cout<<coasts_[i]->getName()<<", ";
    }
    std::cout<<"\n";
}
