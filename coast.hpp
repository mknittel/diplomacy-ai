/**
 * coast.hpp
 *
 * Interface. Defines coasts and how they connect.
 */

#ifndef COAST_HPP_INCLUDED
#define COAST_HPP_INCLUDED

#include <string>
#include <vector>
#include "province.hpp"

class LandProvince;
class SeaProvince;

class Coast {
public:
    /**
     * \brief Defines a coast with no connections
     * to other coasts but is at a province.
     */
    Coast(std::string name, LandProvince* province);

    /**
     * \brief Defines a coast and how it connects
     * to other coasts.
     */
    Coast(std::string name, std::vector<Coast*> connections, LandProvince* province);

    /**
     * \brief Retrieves coast name.
     */
    std::string getName();

    /**
     * \brief Adds connection to a coast.
     */
    void addConnection(Coast* connection);

    /**
     * \brief Adds connection to a sea.
     */
    void addSea(SeaProvince* sea);

    /**
     * \brief Returns a list of coastal connections.
     */
    std::vector<Coast*> getConnections();

    /**
     * \brief Returns a list of adjacent seas.
     */
    std::vector<SeaProvince*> getAdjacentSeas();

    /**
     * \brief Returns the number of connections.
     */
    size_t getNumConnections();

    /**
     * \brief Returns the province this coast is at.
     */
    LandProvince* getProvince();

private:
    std::string name_;
    LandProvince* province_;
    std::vector<Coast*> connections_;
    std::vector<SeaProvince*> adjacentSeas_;
};

#endif // ifndef COAST_HPP_INCLUDED

