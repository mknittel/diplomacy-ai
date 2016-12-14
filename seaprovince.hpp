/**
 * seaprovince.hpp
 *
 * Interface. Defines a sea province on the board.
 * Contains information on its name, if it has
 * a supply center, who controls it, which
 * coasts it has, and which neighbors it has.
 */

#ifndef SEAPROVINCE_HPP_INCLUDED
#define SEAPROVINCE_HPP_INCLUDED

#include <string>
#include <vector>
#include "province.hpp"
#include "coast.hpp"

class SeaProvince: public Province {
public:
    using Province::Province;

    /**
     * \brief Adds a coast adjacent to the province.
     */
    void addCoast(Coast* coast, Province* province);

private:
    std::vector<Coast*> adjacentCoasts_;
};

#endif // ifndef SEAPROVINCE_HPP_INCLUDED

