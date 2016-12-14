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
    void addCoast(Coast* coast, std::vector<Province*> neighbors);   

    /**
     * \brief Updates neighbors given coasts.
     */
    void updateNeighbors();

    /**
     * \brief Returns a list of this province's coasts.
     */
    std::vector<Coast*> getCoasts();

    /**
     * \brief Returns a pointer to the coast with the
     * given name.
     */
    Coast* getCoast(std::string name);

    /**
     * \brief Makes this province a supply center.
     */
    void makeCenter();
    
    void printCoasts();

private:
    std::vector<Coast*> coasts_;
    bool isCenter_ = false;
};

#endif // ifndef LANDPROVINCE_HPP_INCLUDED
