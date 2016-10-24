/**
 * moveparser.hpp
 * 
 * Interface. Parses orders into moves. 
 */

#ifndef MOVEPARSER_HPP_INCLUDED
#define MOVEPARSER_HPP_INCLUDED = 1;

#include <string>
#include <vector>
#include <iostream>
//#include <castor.h>

#include "player.hpp"
#include "province.hpp"
#include "landprovince.hpp"
#include "seaprovince.hpp"
#include "resolver.hpp"

class MoveParser {
public:
    /**
     * \struct Move
     * 
     * \brief Contains information about an
     * attempted move, what units are involved,
     * what locations are involved, and how 
     * much support it has.
     */
/*    struct Move {
        std::string actorType_;
        std::string actorCoast_; // Only if fleet on land
        std::string actorProv_;
        
        enum MoveType { hold, convoy, move, support };

        MoveType moveType_;

        Player::Unit* actor_;
        std::vector<Player::Unit*> convoy_;
        std::vector<Player::Unit*> support_;

        std::vector<Province*> successProvinces_;
        std::vector<Province*> failProvinces_;

        size_t power_;
    };
*/
    // TODO: make string-based structs for each movetype, different strings per situation

    /**
     * \brief Parses move orders into moves.
     */
    void readMoves(std::string filename, std::vector<Province*> provinces);

    /**
     * \brief Resolves previously read moves.
     */
    void resolveMoves();

private:
    std::vector<Resolver::Hold> holds_;
    std::vector<Resolver::Convoy> convoys_;
    std::vector<Resolver::Move> moves_;
    std::vector<Resolver::SupportHold> supportHolds_;
    std::vector<Resolver::SupportConvoy> supportConvoys_;
    std::vector<Resolver::SupportMove> supportMoves_;

    // Finds existing convoy.
    int findConvoy(std::string start, Convoy* convoy);

    // Finds existing hold.
    int findHold(std::string start, Hold* hold);
    
    // Finds existing move.
    int findMove(std::string start, Move* move);

};

#endif // ifndef MOVEPARSER_HPP_INCLUDED
