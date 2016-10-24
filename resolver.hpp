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
    struct Hold {
        std::string prov_;
        std::string player_;
    };

    struct Convoy {
        std::string dest_;
        std::string start_;
        std::vector<std::string> convoy_;
        std::string player_;
    };

    struct Move {
        std::string dest_;
        std::string start_;
        std::string player_;

        std::string coast_; // May or may no exist, refers to destination coast
    };

    struct Support {
        std::string prov_;
        std::string player_;
    };

    struct SupportHold : Support {
        Hold* hold_;
    };

    struct SupportConvoy : Support {
        Convoy* convoy_;
    };

    struct SupportMove : Support {
        Move* move_;
    };

    /**
     * \breif Constructs resolver with given moves, players, provinces.
     */
    Resolver(std::vector<Hold>          holds,
             std::vector<Convoy>        convoys,
             std::vector<Move>          moves,
             std::vector<SupportHold>   supportHolds,
             std::vector<SupportConvoy> supportConvoys,
             std::vector<SupportMove>   supportMoves,
             std::vector<Province*>     provinces,
             std::vector<Player*>       players);

private:
    std::string writeHold(Resolver::Hold hold);
    std::string writeConvoy(Resolver::Convoy convoy, std::string& convoysPred);
    std::string writeMove(Resolver::Move move);
    std::string writeSupportHold(Resolver::SupportHold supportHold);
    std::string writeSupportConvoy(Resolver::SupportConvoy supportConvoy);
    std::string writeSupportMove(Resolver::SupportMove supportMove);
};

#endif // ifndef RESOLVER_HPP_INCLUDED
