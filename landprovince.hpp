/**
 * landprovince.hpp
 *
 * Interface. Defines a land province on the board.
 * Contains information on its name, if it has
 * a supply center, who controls it, which
 * coasts it has, and which neighbors it has.
 */

#ifndef LANDPROVINCE_HPP_INCLUDED
#define LANDPROVINCE_HPP_INCLUDED

#include <string>
#include <vector>
#include "province.hpp"
#include "coast.hpp"

class LandProvince: public Province {
public:
    using Province::Province;

    /**
     * \brief Adds a coast to the province.
     */
    void addCoast(Coast* coast);   

    /**
     * \brief Updates neighbors given coasts.
     */
    void updateNeighbors();

    /**
     * \brief Returns a list of this province's coasts.
     */
    std::vector<Coast*> getCoasts();

    /**
     * \breif Returns a pointer to the coast with the
     * given name.
     *
     * \post Returns 1 if it finds the coast, else 0.
     */
    int getCoast(std::string name, Coast* coast);

    /**
     * \brief Makes this province a supply center.
     */
    void makeCenter();

private:
    std::vector<Coast*> coasts_;
    bool isCenter_ = false;
};

#endif // ifndef LANDPROVINCE_HPP_INCLUDED
