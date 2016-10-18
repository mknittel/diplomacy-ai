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
#include "player.hpp"
#include "landprovince.hpp"
#include "coast.hpp"

void LandProvince::addCoast(Coast* coast)
{
    coasts_.push_back(coast);

    std::vector<Coast*> connections = coast->getConnections();
    size_t size = connections.size();

    for (size_t i = 0; i < size; ++i) {
        Coast* connection = connections[i];
        LandProvince* province = connection->getProvince();
        
        if (!hasNeighbor(province)) {
            addNeighbor(province);
        }
    }
}

void LandProvince::updateNeighbors()
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
}

std::vector<Coast*> LandProvince::getCoasts()
{
    return coasts_;
}

int LandProvince::getCoast(std::string name, Coast* coast)
{
    for (size_t i = 0; i < coasts_.size(); ++i) {
        if (coasts_[i]->getName() == name) {
            coast = coasts_[i];
            return 1;
        }
    }

    return 0;
}

void LandProvince::makeCenter()
{
    isCenter_ = true;
}
