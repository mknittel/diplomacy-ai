/**
 * board.cpp
 * 
 * Implementation. Keeps track of the board set up
 * and its current state.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <iterator>
#include <algorithm>
#include "board.hpp"

static std::vector<std::string> split(std::string input, char delimeter)
{
    std::string nextString;
    std::istringstream ss(input);
    std::vector<std::string> splitLine;

    while (std::getline(ss, nextString, delimeter)) {
        if (nextString[0] == ' ') {
            nextString = nextString.substr(1, nextString.size() - 1);
        }

        splitLine.push_back(nextString);
    } 

    return splitLine;
}

Board::Board()
{
    Player neutral("Neutral");
    players_.push_back(neutral);
}

void Board::uploadBoard(std::string filename)
{
    std::vector<std::vector<std::string>> landProvinces;
    std::vector<std::vector<std::string>> seaProvinces;
    std::vector<std::vector<std::string>> coastStrings;
    std::vector<std::vector<std::string>> playerStrings;
    std::vector<std::string> centerStrings;
    
    std::cout<<"preinterpret\n";

    interpretFile(filename, &landProvinces, &seaProvinces, &coastStrings,
                  &playerStrings, &centerStrings);

    std::cout<<"postinterpret\n";

    // Populate players
    for (size_t i = 0; i < playerStrings.size(); ++i) {
        Player player(playerStrings[i][0]);
        players_.push_back(player);
    } 

    // Populate land provinces
    for (size_t i = 0; i < landProvinces.size(); ++i) {
        LandProvince province(landProvinces[i][0]);
        landProvinces_.push_back(province); 
    }

    // Populate sea provinces
    for (size_t i = 0; i < seaProvinces.size(); ++i) {
        SeaProvince province(seaProvinces[i][0]);
        seaProvinces_.push_back(province);
    }

    std::cout<<"postpopulation\n";

    // Populate coasts
    for (size_t i = 0; i < coastStrings.size(); ++i) {
        int ind = getLandProvince(coastStrings[i][0]);
        
        if (ind >= 0) {
            LandProvince* province = &landProvinces_[ind];
            
            Coast coast;
            coast.setName(coastStrings[i][1]);
            coast.setProvince(province->getName());
            coasts_.push_back(coast);

            std::vector<Coast*> connections = coast.getConnections();
            std::vector<Province*> neighbors;

            for (size_t j = 0; j < connections.size(); ++j) {
                Coast* connection = connections[i];
                std::string provinceName = connection->getProvince();
                int ind2 = getProvince(provinceName);
                Province* province;

                if (ind2 >= landProvinces_.size()) {
                    province = &seaProvinces_[ind2 - landProvinces_.size()];
                } else {
                    province = &landProvinces_[ind2];
                }

                neighbors.push_back(province);
            }

            landProvinces_[ind].addCoast(&(coasts_[i]), neighbors);

            if (province->getName() == "Ven") {
                std::cout << "Print from array";
                int ind2 = getLandProvince(coastStrings[i][0]);
                LandProvince* prv = &landProvinces_[ind2]; 
                prv->printCoasts();

                std::cout << "Coasts of Ven: ";
                int ind3 = getLandProvince("Ven");
                LandProvince* ven = &landProvinces_[ind3];
                ven->printCoasts();
            }
        } else {
            std::cout << "Error: coast linked to invalid province.\n";
            return;
        }

    }
    std::cout << "Coasts of Ven: ";
    int ind4 = getLandProvince("Ven");
    LandProvince* ven = &landProvinces_[ind4]; 
    ven->printCoasts();

    for (size_t i = 0; i < coasts_.size(); ++i) {
        std::cout << coasts_[i].getName() << ", ";
    }
    std::cout << "\n";

    std::cout << "Coasts of Ven: ";
    ven->printCoasts();

    std::cout<<"1\n";
    linkCoasts(coastStrings);
    std::cout<<"2\n";
    linkLand(landProvinces);
    std::cout<<"3\n";
    linkSeas(seaProvinces);
    std::cout<<"4\n";
    addCenters(centerStrings);
    std::cout<<"5\n";
    setHomes(playerStrings);
    std::cout<<"6\n";

    //updateLandNeighbors();
}

std::vector<Player::Retreat> Board::executeMoves(std::string filename)
{
    std::vector<Province*> provinces;

    for (size_t i = 0; i < landProvinces_.size(); ++i) {
        provinces.push_back(&(landProvinces_[i]));
    }

    for (size_t i = 0; i < seaProvinces_.size(); ++i) {
        provinces.push_back(&(seaProvinces_[i]));
    }

    std::vector<Player*> players;

    for (size_t i = 0; i < players_.size(); ++i) {
        players.push_back(&(players_[i]));
    }

    MoveParser parser(provinces, players);
    parser.readMoves(filename);
    Player::MoveSet moveSet = parser.resolveMoves();

    for (size_t i = 0; i < moveSet.convoys_.size(); ++i) {
        std::string start = moveSet.convoys_[i].start_;
        std::string dest = moveSet.convoys_[i].dest_;    
        std::string playerName = moveSet.convoys_[i].player_;

        modifyBoard(start, dest, playerName);
    }

    for (size_t i = 0; i < moveSet.moves_.size(); ++i) {
        std::string start = moveSet.moves_[i].start_;
        std::string dest = moveSet.moves_[i].dest_;    
        std::string playerName = moveSet.moves_[i].player_;

        modifyBoard(start, dest, playerName);
    }

    return moveSet.retreats_;
}

void Board::executeRetreats(std::string filename)
{
    MoveParser parser;
    std::vector<Player::Retreat> retreats = parser.parseRetreats(filename);
    
    for (size_t i = 0; i < retreats.size(); ++i) {
        int ind = getPlayer(retreats[i].player_);
        Player* player = &players_[ind]; 

        int ind2 = getProvince(retreats[i].retreat_);
        Province* province;

        if (ind2 >= landProvinces_.size()) {
            province = &seaProvinces_[ind2 - landProvinces_.size()];
        } else {
            province = &landProvinces_[ind2];
        }

        player->moveUnit(retreats[i].prov_, province);
    }
}

void Board::executeBuilds(std::string filename)
{
    MoveParser parser;
    std::vector<Player::Build> builds = parser.parseBuilds(filename);
    
    for (size_t i = 0; i < builds.size(); ++i) {
        int ind = getPlayer(builds[i].player_);
        Player* player = &players_[ind];

        int ind2 = getProvince(builds[i].prov_);
        Province* province;

        if (ind2 >= landProvinces_.size()) {
            province = &seaProvinces_[ind2 - landProvinces_.size()];
        } else {
            province = &landProvinces_[ind2];
        }

        player->placeUnit((LandProvince*) province, builds[i].type_, builds[i].coast_);
    }
}

void Board::modifyBoard(std::string start, std::string dest, std::string playerName)
{
    int ind = getPlayer(playerName);
    Player* player = &players_[ind];

    int ind2 = getProvince(start);
    Province* province;

    if (ind2 >= landProvinces_.size()) {
        province = &seaProvinces_[ind2 - landProvinces_.size()];
    } else {
        province = &landProvinces_[ind2];
    }

    player->moveUnit(start, province);
    
    if (province->hasCenter()) {
        std::string loserName = province->getController();
        int ind3 = getPlayer(loserName);
        Player* loser = &players_[ind3];
        
        loser->removeCenter((LandProvince*) province);
    }

    province->setController(playerName);
}

/*void Board::updateLandNeighbors()
{
    for (size_t i = 0; i < landProvinces_.size(); ++i) {
        landProvinces_[i].updateNeighbors();
    }
}*/

int Board::getLandProvince(std::string name)
{
    for (size_t i = 0; i < landProvinces_.size(); ++i) {
        if (name == landProvinces_[i].getName()) {
            return i;
        }
    }

    return -1;
}

int Board::getSeaProvince(std::string name)
{
    for (size_t i = 0; i < seaProvinces_.size(); ++i) {
        if (name == seaProvinces_[i].getName()) {
            return i;
        }
    }

    return -1;
}

int Board::getProvince(std::string name)
{
    for (size_t i = 0; i < landProvinces_.size(); ++i) {
        if (name == landProvinces_[i].getName()) {
            return i;
        }
    }
    
    for (size_t i = 0; i < seaProvinces_.size(); ++i) {
        if (name == seaProvinces_[i].getName()) {
            return i + landProvinces_.size();
        }
    }

    return -1;
}

int Board::getPlayer(std::string name)
{
    for (size_t i = 0; i < players_.size(); ++i) {
        if (name == players_[i].getName()) {
            return i;
        }
    }
    
    return -1;
}

void Board::interpretFile(std::string                            filename,
                          std::vector<std::vector<std::string>>* landProvinces,
                          std::vector<std::vector<std::string>>* seaProvinces,
                          std::vector<std::vector<std::string>>* coastStrings,
                          std::vector<std::vector<std::string>>* playerStrings,
                          std::vector<std::string>*              centerStrings)
{ 
    std::ifstream file{filename};

    std::string nextLine("");

    while (std::getline(file, nextLine)) {
        if (nextLine.substr(0,8) == "Players:") {
            while (std::getline(file, nextLine) && nextLine != "-") {
                std::vector<std::string> splitLine(split(nextLine, ','));
                playerStrings->push_back(splitLine);
            }
        } else if (nextLine.substr(0,15) == "Land provinces:") {
            while (std::getline(file, nextLine) && nextLine != "-") {
                std::vector<std::string> splitLine(split(nextLine, ','));
                landProvinces->push_back(splitLine);
            }
        } else if (nextLine.substr(0,14) == "Sea provinces:") {
            while (std::getline(file, nextLine) && nextLine != "-") {
                std::vector<std::string> splitLine(split(nextLine, ','));
                seaProvinces->push_back(splitLine);
            }
        } else if (nextLine.substr(0,7) == "Coasts:") {
            while (std::getline(file, nextLine) && nextLine != "-") {
                std::vector<std::string> splitLine(split(nextLine, ','));
                coastStrings->push_back(splitLine);
            }
        } else if (nextLine.substr(0,23) == "Neutral Supply Centers:") {
            while (std::getline(file, nextLine) && nextLine != "-") {
                centerStrings->push_back(nextLine);
            }
        }
    }

    file.close();
}

void Board::linkCoasts(std::vector<std::vector<std::string>> coastStrings)
{
    for (size_t i = 0; i < coastStrings.size(); ++i) {
        std::vector<std::string> coastString = coastStrings[i];
    
        for (size_t j = 2; j < coastString.size(); ++j) {
            std::string provinceName(coastString[j]);
            int ind = getLandProvince(provinceName);

            if (ind >= 0) {
                LandProvince* landProvince = &landProvinces_[ind];
                std::string coastName = coastString[j + 1];
                Coast* coast = landProvince->getCoast(coastName);
            
                if (coast != nullptr) {
                    std::cout << "Successful coast link\n";
                    coasts_[i].addConnection(coast);
                    ++j;
                } else {
                    std::cout << "Error: coast "
                              << coasts_[i].getName()
                              << " of province "
                              << coasts_[i].getProvince()
                              << " linked to invalid coast "
                              << coastName << " of province "
                              << provinceName << ".\n";
                    return;
                }
            } else {
                int ind2 = getSeaProvince(provinceName);
        
                if (ind2 >= 0) {
                    SeaProvince* seaProvince = &seaProvinces_[ind2]; 
                    coasts_[i].addSea(seaProvince->getName());
                } else {
                    std::cout << "Error: coast linked to invalid province.\n";
                    return;
                }
            }
        }
    }
}

void Board::linkLand(std::vector<std::vector<std::string>> landProvinces)
{
    for (size_t i = 0; i < landProvinces.size(); ++i) {
        std::vector<std::string> provinces = landProvinces[i];
        
        for (size_t j = 1; j < provinces.size(); ++j) {
            std::string provinceName = provinces[j];
            int ind = getLandProvince(provinceName);

            if (ind >= 0) {
                LandProvince* province = &landProvinces_[ind]; 
                landProvinces_[i].addNeighbor(province);
            } else {
                std::cout << "Error: land province linked to invalid province.\n";
                return;
            }
        }
    }
}

void Board::linkSeas(std::vector<std::vector<std::string>> seaProvinces)
{
    for (size_t i = 0; i < seaProvinces.size(); ++i) {
        std::vector<std::string> provinces = seaProvinces[i];
        
        for (size_t j = 0; j < provinces.size(); ++j) {
            std::string provinceName = provinces[j];
            int ind = getLandProvince(provinceName);

            if (ind >= 0) {
                LandProvince* landProvince = &landProvinces_[ind];
                std::string coastName = provinces[j + 1]; 
                Coast* coast = landProvince->getCoast(coastName);
            
                if (coast != nullptr) {
                    seaProvinces_[i].addCoast(coast, (Province*) landProvince);
                    ++j;
                } else {
                    std::cout << "Error: sea linked to invalid coast.\n";
                    return;
                }
            } else {
                int ind2 = getSeaProvince(provinceName);
        
                if (ind2 >= 0) {
                    SeaProvince* seaProvince = &seaProvinces_[ind2];
                    seaProvinces_[i].addNeighbor(seaProvince);
                } else {
                    std::cout << "Error: sea linked to invalid province.\n";
                    return;
                }
            }
        } 
    }
}

void Board::setHomes(std::vector<std::vector<std::string>> playerStrings)
{
    for (size_t i = 0; i < playerStrings.size(); ++i) {
        std::vector<std::string> playerString = playerStrings[i];

        for (size_t j = 1; j < playerString.size(); ++j) {
            std::string provinceName = playerString[j];

            std::cout<<"province: "<< provinceName<<"\n";

            int ind = getLandProvince(provinceName);

            if (ind >= 0) {
                LandProvince* province = &landProvinces_[ind];
                players_[i].addHomeCenter(province);
                
                std::string playerName = players_[i].getName();
                province->setController(playerName);
            } else {
                std::cout << "Error: invalid home for player.\n";
                return;
            }
        }
    }
}

void Board::addCenters(std::vector<std::string> centerStrings)
{
    for (size_t i = 0; i < centerStrings.size(); ++i) {
        std::string center = centerStrings[i];
        int ind = getLandProvince(center);

        if (ind >= 0) {
            LandProvince* landProvince = &landProvinces_[ind];
            landProvince->makeCenter();
        } else {
            std::cout << "Error: supply center on invalid province.\n";
            return;
        }
    }
}

size_t Board::getNumCenters()
{
    size_t numCenters = 0;

    for (size_t i = 0; i < landProvinces_.size(); ++i) {
        if (landProvinces_[i].hasCenter()) {
            ++numCenters;
        }
    }
    
    return numCenters;
}

std::vector<Player*> Board::getPlayers()
{
    std::vector<Player*> players;

    for (size_t i = 0; i < players_.size(); ++i) {
        players.push_back(&players_[i]);
    }

    return players;
}
