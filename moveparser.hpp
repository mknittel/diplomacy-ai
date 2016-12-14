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

#include "player.hpp"
#include "province.hpp"
#include "landprovince.hpp"
#include "seaprovince.hpp"
#include "resolver.hpp"

class MoveParser {
public:
    /**
     * \brief Constructs parser with no provinces or players.
     */
    MoveParser();

    /**
     * \brief Constructs parser with provinces and players.
     */
    MoveParser(std::vector<Province*> provinces, std::vector<Player*> players);

    /**
     * \brief Parses move orders into moves.
     */
    void readMoves(std::string filename);

    /**
     * \brief Resolves and executes previously read moves.
     */
    Player::MoveSet resolveMoves();

    /**
     * \brief Parses retreat orders from file.
     */
    std::vector<Player::Retreat> parseRetreats(std::string filename);

    /**
     * \brief Parses build orders from file.
     */
    std::vector<Player::Build> parseBuilds(std::string filename);

private:
    Player::MoveSet moveSet_;
    std::vector<Province*> provinces_;
    std::vector<Player*> players_;

    // Finds existing convoy.
    int findConvoy(std::string start, Player::Convoy* convoy);

    // Finds existing hold.
    int findHold(std::string start, Player::Hold* hold);
    
    // Finds existing move.
    int findMove(std::string start, Player::Move* move);

    // Gets resolutions
    Player::MoveSet getResolutions(std::vector<std::string> resolutions);
};

#endif // ifndef MOVEPARSER_HPP_INCLUDED
