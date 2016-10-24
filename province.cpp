/**
 * province.cpp
 *
 * Implementation. Defines a province on the board.
 * Contains information on its name, if it has
 * a supply center, who controls it, which
 * coasts it has, and which neighbors it has.
 */

#include <iostream>
#include <string>

#include "province.hpp"

Province::Province(std::string name)
: name_(name),
  hasCenter_(false),
  neighbors_(5),
  numNeighbors_(0)
{

}

void Province::addNeighbor(Province* neighbor)
{
    neighbors_.push_back(neighbor);
}

size_t Province::getNumNeighbors()
{
    return neighbors_.size();
}

std::string Province::getName()
{
    return name_;
}

bool Province::hasNeighbor(Province* province)
{
    size_t size = neighbors_.size();

    for (size_t i = 0; i < size; ++i) {
        if (province->name_ == neighbors_[i]->getName()) {
            return true;
        }
    }

    return false;
}
