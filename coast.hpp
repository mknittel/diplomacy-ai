/**
 * coast.hpp
 *
 * Interface. Defines coasts and how they connect.
 */

#ifndef COAST_HPP_INCLUDED
#define COAST_HPP_INCLUDED

#include <string>
#include <vector>

class Coast {
public:
    Coast();
    /**
     * \brief Defines a coast with no connections
     * to other coasts but is at a province.
     */
    Coast(std::string name, std::string province);

    /**
     * \brief Defines a coast and how it connects
     * to other coasts.
     */
    Coast(std::string name, std::vector<Coast*> connections, std::string province);

    /**
     * \brief Sets coast name.
     */
    void setName(std::string name);

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
    void addSea(std::string sea);

    /**
     * \brief Returns a list of coastal connections.
     */
    std::vector<Coast*> getConnections();

    /**
     * \brief Returns a list of adjacent seas.
     */
    std::vector<std::string> getAdjacentSeas();

    /**
     * \brief Returns the number of connections.
     */
    size_t getNumConnections();
    
    /**
     * \brief Sets the province of the coast.
     */
    void setProvince(std::string province);

    /**
     * \brief Returns the province name this coast is at.
     */
    std::string getProvince();

private:
    std::string name_;
    std::string province_;
    std::vector<Coast*> connections_;
    std::vector<std::string> adjacentSeas_;
};

#endif // ifndef COAST_HPP_INCLUDED

