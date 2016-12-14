/**
 * coast.cpp
 *
 * Implementation. Defines coasts and how they connect.
 */

#include <string>
#include <vector>
#include <iostream>

#include "coast.hpp"

Coast::Coast() {}

Coast::Coast(std::string name, std::string province)
: name_(name),
  connections_(0),
  province_(province)
{

}

Coast::Coast(std::string name, std::vector<Coast*> connections, std::string province)
: name_(name),
  connections_(connections),
  province_(province)
{

}

void Coast::setName(std::string name) { name_ = name; }

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

std::vector<std::string> Coast::getAdjacentSeas()
{
    return adjacentSeas_;
}

void Coast::setProvince(std::string province)
{
    province_ = province;
}

std::string Coast::getProvince()
{
    return province_;
}

void Coast::addSea(std::string seaProvince)
{
    adjacentSeas_.push_back(seaProvince);
}
