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
#include "castor1.1/castor.h"

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
        if (provinces[i].getName() == name) {
            return true;
        }
    }
}

int MoveParser::findConvoy(std::string start, Resolver::Convoy* convoy)
{
    for (size_t i = 0; i < convoys_.size(); ++i) {
        if (start == convoys[i]->start_) {
            convoy = &(convoys[i]);
            return 1;
    }

    return 0;
}

int MoveParser::findHold(std::string start, Resolver::Hold* hold)
{
    for (size_t i = 0; i < holds_.size(); ++i) {
        if (start == holds[i]->start_) {
            hold = &(holds[i]);
            return 1;
    }

    return 0;
}

int MoveParser::findMove(std::string start, Resolver::Move* hold)
{
    for (size_t i = 0; i < holds_.size(); ++i) {
        if (start == holds[i]->start_) {
            hold = &(holds[i]);
            return 1;
    }

    return 0;
}

void MoveParser::readMoves(std::string filename, std::vector<Province*> provinces)
{
    std::vector<Resolver::Move> moves;

    std::ifstream file{filename};
    std::string nextLine("");
    file >> nextLine;

    while (nextLine != "endoffile") {
        std::string player = nextLine;
        file >> nextLine;

        while (nextLine != "") {
            std::vector<std::string> words(split(nextline, " "));
            std::string type;
            std::string coast; 

            // 1st or second word is province, check which
            if (isProvinceName(words[1]), provinces) {
                type = words[2];
            } else {
                type = words[3];
                coast = words[1];
                words.erase(words.begin() + 1); // Remove coast word
            }

            if (type == "hold") {
                Resolver::Hold hold;
                hold.prov_ = words[1];
                hold.player_ = player;

                holds_.push_back(hold);
            } else if (type == "convoy") {
                Resolver::Convoy* searchConvoy;

                if (findConvoy(words[3], searchConvoy) == 1) {
                    searchConvoy->convoy_.push_back(words[1]);
                } else {
                    Resolver::Convoy convoy;
                    convoy.start_ = words[4];
                    convoy.dest_ = words[6];
                    convoy.player_ = player;
                    convoy.convoy_.push_back(words[1];

                    convoys_.push_back(convoy);
                }
            } else if (type == "to") {
                Resolver::Move move;
                move.start_ = words[1];
                move.dest_ = words[3];
                move.player_ = player;

                moves_.push_back(move);
            } else if (type == "support") {
                std::string supportType = words[5];
                std::string province = words[1];
                std::vector<std::string> supportedStart = words[3];

                if (supportType == "hold") {
                    Resolver::Hold* hold;
                    findHold(supportedStart, hold);

                    Resolver::SupportHold support;
                    support.prov_ = province;
                    support.player_ = player;
                    support.hold_ = hold;

                    supportHolds_.push_back(support);
                } else if (supportType == "convoy") {
                    Resolver::Convoy* convoy;
                    findConvoy(supportedStart, convoy);

                    Resolver::SupportConvoy support;
                    support.prov_ = province;
                    support.player_ = player;
                    support.convoy_ = convoy;

                    supportConvoys_.push_back(support);
                } else {
                    Resolver::Move* move;
                    findMove(supportedStart, move);

                    Resolver::SupportMove support;
                    support.prov_ = province;
                    support.player_ = player;
                    support.move_ = move;

                    supportMoves_.push_back(support);
                }
            }

            file >> nextLine;
        }
    }
    

    return moves;
}

void MoveParser::resolveMoves()
{
    
}

int main(int argc, char** argv)
{
    return 1;
}
