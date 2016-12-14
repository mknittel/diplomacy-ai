/**
 * resolver.hpp
 *
 * Interface. Utilizes logical programming
 * to resolve given moves.
 */

#ifndef RESOLVER_HPP_INCLUDED
#define RESOLVER_HPP_INCLUDED

#include <string>
#include <iostream>
#include <vector>
#include "province.hpp"
#include "player.hpp"

class Resolver {
public:
    /**
     * \brief Constructs resolver with given moves, players, provinces.
     */
    Resolver(Player::MoveSet        moveSet,
             std::vector<Province*> provinces,
             std::vector<Player*>   players);

    /**
     * \brief Resolves moves.
     */
    std::vector<std::string> resolve();

private:
    std::string writeHold(Player::Hold hold);
    std::string writeConvoy(Player::Convoy convoy, std::string& convoysPred);
    std::string writeMove(Player::Move move);
    std::string writeSupportHold(Player::SupportHold supportHold);
    std::string writeSupportConvoy(Player::SupportConvoy supportConvoy);
    std::string writeSupportMove(Player::SupportMove supportMove);

    //Player::MoveSet makeMoveSet(std::vector<std::string> moveStrings);
};

#endif // ifndef RESOLVER_HPP_INCLUDED
