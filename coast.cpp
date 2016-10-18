/**
 * coast.cpp
 *
 * Implementation. Defines coasts and how they connect.
 */

#include <string>
#include <vector>

#include "coast.hpp"

Coast::Coast(std::string name, LandProvince* province)
: name_(name),
  connections_(2),
  province_(province)
{

}

Coast::Coast(std::string name, std::vector<Coast*> connections, LandProvince* province)
: name_(name),
  connections_(connections),
  province_(province)
{

}

std::string Coast::getName() { return name_; }

void Coast::addConnection(Coast* connection)
{
    connections_.push_back(connection);
}

size_t Coast::getNumConnections()
{
    return connections_.size();
}

std::vector<Coast*> Coast::getConnections()
{
    return connections_;
}

std::vector<SeaProvince*> Coast::getAdjacentSeas()
{
    return adjacentSeas_;
}

LandProvince* Coast::getProvince()
{
    return province_;
}

void Coast::addSea(SeaProvince* seaProvince)
{
    adjacentSeas_.push_back(seaProvince);
}
