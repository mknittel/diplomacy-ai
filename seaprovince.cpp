/**
 * seaprovince.hpp
 *
 * Implementation. Defines a sea province on the board.
 * Contains information on its name, if it has
 * a supply center, who controls it, which
 * coasts it has, and which neighbors it has.
 */

#include <string>
#include <vector>
#include "player.hpp"
#include "seaprovince.hpp"
#include "coast.hpp"

void SeaProvince::addCoast(Coast* coast)
{
    adjacentCoasts_.push_back(coast);
    addNeighbor(new SeaProvince("hi"));
}
