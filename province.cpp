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

Province::Province(std::string name, bool hasCenter,
                    Player controller, int coasts[])
: name_(name),
  hasCenter_(hasCenter),
  controller_(controller),
  coasts_(coasts),
  neighbors_(new Province*[5]),
  numNeighbors_(0)
{

}
