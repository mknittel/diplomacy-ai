/**
 * province.hpp
 *
 * Interface. Defines a province on the board.
 * Contains information on its name, if it has
 * a supply center, who controls it, which
 * coasts it has, and which neighbors it has.
 */

#ifndef PROVINCE_HPP_INCLUDED
#define PROVINCE_HPP_INCLUDED

#include <string>
#include <vector>

class Province {
public:
    /**
     * \brief Creates a province with the given name.
     */
    Province(std::string name);

    /**
     * \breif Checks if this province has this neighbor.
     */
    bool hasNeighbor(Province* province);

    /**
     * \brief Adds a neighbor province.
     */
    void addNeighbor(Province* neighbor);

    /**
     * \breif Returns the total number of 
     * neighbors.
     */
    size_t getNumNeighbors();

    /**
     * \brief Returns this province's name.
     */
    std::string getName();

private:
    std::string name_;
    bool hasCenter_;
    std::vector<Province*> neighbors_;
    size_t numNeighbors_;
};

#endif // ifndef PROVINCE_HPP_INCLUDED

