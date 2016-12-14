/**
 * moveparser.cpp
 *
 * Implementation. Parses orders into moves.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <iterator>
#include <algorithm>
#include "moveparser.hpp"
#include "resolver.hpp"

static std::vector<std::string> split(std::string input, std::string delimeter)
{
    std::string word = "";
    std::vector<std::string> splitString(40);

    for (size_t i = 0; i < input.size(); ++i) {
        if (input.substr(i,delimeter.size()) == delimeter) {
            splitString.push_back(word);
            word = "";
            i += delimeter.size() - 1;
        } else {
            word += input[i]; 
        }
    }

    splitString.push_back(word);    

    return splitString;
}

static bool isProvinceName(std::string name, std::vector<Province*> provinces)
{
    for (size_t i = 0; i < provinces.size(); ++i) {
        if (provinces[i]->getName() == name) {
            return true;
        }
    }
}

static bool contains(std::string container, std::string containee)
{
    for (size_t i = 0; i < container.size() - containee.size(); ++i) {
        if (container.substr(i, containee.size()) == containee) {
            return true;
        }
    }

    return false;
}

MoveParser::MoveParser()
{
}

MoveParser::MoveParser(std::vector<Province*> provinces, std::vector<Player*> players)
: provinces_(provinces),
  players_(players)
{

}

int MoveParser::findConvoy(std::string start, Player::Convoy* convoy)
{
    for (size_t i = 0; i < moveSet_.convoys_.size(); ++i) {
        if (start == moveSet_.convoys_[i].start_) {
            convoy = &(moveSet_.convoys_[i]);
            return 1;
        }
    }

    return 0;
}

int MoveParser::findHold(std::string start, Player::Hold* hold)
{
    for (size_t i = 0; i < moveSet_.holds_.size(); ++i) {
        if (start == moveSet_.holds_[i].prov_) {
            hold = &(moveSet_.holds_[i]);
            return 1;
        }
    }

    return 0;
}

int MoveParser::findMove(std::string start, Player::Move* move)
{
    for (size_t i = 0; i < moveSet_.moves_.size(); ++i) {
        if (start == moveSet_.moves_[i].start_) {
            move = &(moveSet_.moves_[i]);
            return 1;
        }
    }

    return 0;
}

void MoveParser::readMoves(std::string filename)
{
    Player::MoveSet moveSet; 
    moveSet_ = moveSet;

    std::ifstream file{filename};
    std::string nextLine = "";
    file >> nextLine;

    while (nextLine != "endoffile") {
        std::string player = nextLine;
        file >> nextLine;

        while (nextLine != "") {
            std::vector<std::string> words(split(nextLine, " "));
            std::string type;

            if (type == "hold") {
                Player::Hold hold;
                hold.prov_ = words[1];
                hold.player_ = player;

                moveSet_.holds_.push_back(hold);
            } else if (type == "convoy") {
                Player::Convoy* searchConvoy;

                if (findConvoy(words[3], searchConvoy) == 1) {
                    searchConvoy->convoy_.push_back(words[1]);
                } else {
                    Player::Convoy convoy;
                    convoy.start_ = words[4];
                    convoy.dest_ = words[6];
                    convoy.player_ = player;
                    convoy.convoy_.push_back(words[1]);

                    moveSet_.convoys_.push_back(convoy);
                }
            } else if (type == "to") {
                Player::Move move;
                move.start_ = words[1];
                move.dest_ = words[3];
                move.player_ = player;

                moveSet_.moves_.push_back(move);
            } else if (type == "support") {
                std::string supportType = words[5];
                std::string province = words[1];
                std::string supportedStart = words[3];

                if (supportType == "hold") {
                    Player::Hold* hold;
                    findHold(supportedStart, hold);

                    Player::SupportHold support;
                    support.prov_ = province;
                    support.player_ = player;
                    support.hold_ = hold;

                    moveSet_.supportHolds_.push_back(support);
                } else if (supportType == "convoy") {
                    Player::Convoy* convoy;
                    findConvoy(supportedStart, convoy);

                    Player::SupportConvoy support;
                    support.prov_ = province;
                    support.player_ = player;
                    support.convoy_ = convoy;

                    moveSet_.supportConvoys_.push_back(support);
                } else {
                    Player::Move* move;
                    findMove(supportedStart, move);

                    Player::SupportMove support;
                    support.prov_ = province;
                    support.player_ = player;
                    support.move_ = move;

                    moveSet_.supportMoves_.push_back(support);
                }
            }

            file >> nextLine;
        }
    }
}

std::vector<Player::Retreat> MoveParser::parseRetreats(std::string filename)
{
    std::ifstream file{filename};
    std::string nextLine("");
    file >> nextLine;

    std::vector<Player::Retreat> retreats;

    while (nextLine != "endoffile") {
        std::vector<std::string> words(split(nextLine, " "));

        Player::Retreat retreat;
        retreat.player_ = words[0];
        retreat.prov_ = words[1];
        retreat.retreat_ = words[2];

        retreats.push_back(retreat);
    }
    
    return retreats;
}

std::vector<Player::Build> MoveParser::parseBuilds(std::string filename)
{
    std::ifstream file{filename};
    std::string nextLine("");
    file >> nextLine;

    std::vector<Player::Build> builds;

    while (nextLine != "endoffile") {
        std::vector<std::string> words(split(nextLine, " "));

        Player::Build build;
        build.player_ = words[0];
        build.type_ = words[1];
        build.prov_ = words[2];

        if (words.size() > 3) {
            build.coast_ = words[3];
        }

        builds.push_back(build);
    }
    
    return builds;
}



Player::MoveSet MoveParser::resolveMoves()
{
    Resolver resolver(moveSet_, provinces_, players_); 
    std::vector<std::string> resolutions = resolver.resolve();

    return getResolutions(resolutions);
}

Player::MoveSet MoveParser::getResolutions(std::vector<std::string> resolutions)
{
    std::string message = "";
    Player::MoveSet moveSet;

    for (size_t i = 0; i < resolutions.size(); ++i) {
        // If it is a dummy string, continue
        if (contains(resolutions[i], "dummy")) {
            continue;
        }

        if (resolutions[i].substr(0, 7) == "retreat") {
            std::string retreatString = resolutions[i].substr(8, resolutions[i].size() - 1); 
            std::vector<std::string> retreatElements = split(retreatString, ", ");

            Player::Retreat retreat;
            retreat.prov_ = retreatElements[0];
            retreat.player_ = retreatElements[1];

            moveSet.retreats_.push_back(retreat);
        } else if (resolutions[i].substr(0,7) == "success"){
            std::string itemString =
                    resolutions[i].substr(8, resolutions[i].size() - 1);

            if (itemString.substr(0,6) == "convoy") {
                std::string convoyString =
                        itemString.substr(6, itemString.size() - 1);
                std::vector<std::string> convoyElements = split(convoyString, ", ");
                
                // Fleets unnecessary
                Player::Convoy convoy;
                convoy.dest_ = convoyElements[0];
                convoy.start_ = convoyElements[1];
                convoy.player_ = convoyElements[3];

                moveSet.convoys_.push_back(convoy);
            } else if (itemString.substr(0,4) == "move") {
                std::string moveString =
                        itemString.substr(4, itemString.size() - 1);
                std::vector<std::string> moveElements = split(moveString, ", ");
                
                Player::Move move;
                move.dest_ = moveElements[0];
                move.start_ = moveElements[1];
                move.player_ = moveElements[2];

                moveSet.moves_.push_back(move);
            }
        } 
    }

    return moveSet;
}
