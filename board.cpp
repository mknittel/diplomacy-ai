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

    interpretFile(filename, &landProvinces, &seaProvinces, &coastStrings,
                  &playerStrings, &centerStrings);

    // Populate plyaers
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

    std::vector<Coast> coasts;

    // Populate coasts
    for (size_t i = 0; i < coastStrings.size(); ++i) {
        LandProvince* province;
        int status(getLandProvince(coastStrings[i][0], province));
        
        if (status == 1) {
            Coast coast(coastStrings[i][1], province);
            coasts.push_back(coast);
            province->addCoast(&coast);
        } else {
            std::cout << "Error: coast linked to invalid province.\n";
        }
    }

    linkCoasts(coastStrings, &coasts);
    linkLand(landProvinces);
    linkSeas(seaProvinces);
    addCenters(centerStrings);

    updateLandNeighbors();
}

void Board::updateLandNeighbors()
{
    for (size_t i = 0; i < landProvinces_.size(); ++i) {
        landProvinces_[i].updateNeighbors();
    }
}

int Board::getLandProvince(std::string name, LandProvince* province)
{
    for (size_t i = 0; i < landProvinces_.size(); ++i) {
        if (name == landProvinces_[i].getName()) {
            province = &landProvinces_[i];
            return 1;
        }
    }

    return 0;
}

int Board::getSeaProvince(std::string name, SeaProvince* province)
{
    for (size_t i = 0; i < seaProvinces_.size(); ++i) {
        if (name == seaProvinces_[i].getName()) {
            province = &seaProvinces_[i];
            return 1;
        }
    }

    return 0;
}

void Board::interpretFile(std::string filename,
                          std::vector<std::vector<std::string>>* landProvinces,
                          std::vector<std::vector<std::string>>* seaProvinces,
                          std::vector<std::vector<std::string>>* coastStrings,
                          std::vector<std::vector<std::string>>* playerStrings,
                          std::vector<std::string>*              centerStrings)
{ 
    std::ifstream file{filename};

    std::string nextLine("");
    file >> nextLine;

    while (nextLine != "endoffile") {
        if (nextLine.substr(0,8) == "Players:") {
            file >> nextLine;

            while (nextLine != "") {
                std::vector<std::string> splitLine(split(nextLine, ", "));
                playerStrings->push_back(splitLine);

                file >> nextLine;
            }
        } else if (nextLine.substr(0,15) == "Land provinces:") {
            file >> nextLine;

            while (nextLine != "") {
                std::vector<std::string> splitLine(split(nextLine, ", "));
                landProvinces->push_back(splitLine);

                file >> nextLine;
            }
        } else if (nextLine.substr(0,14) == "Sea provinces:") {
            file >> nextLine;

            while (nextLine != "") {
                std::vector<std::string> splitLine(split(nextLine, ", "));
                seaProvinces->push_back(splitLine);

                file >> nextLine;
            }
        } else if (nextLine.substr(0,7) == "Coasts:") {
            file >> nextLine;

            while (nextLine != "") {
                std::vector<std::string> splitLine(split(nextLine, ", "));
                coastStrings->push_back(splitLine);

            file >> nextLine;
            }
        } else if (nextLine.substr(0,23) == "Neutral Supply Centers:") {
            file >> nextLine;

            while (nextLine != "") {
                centerStrings->push_back(nextLine);

            file >> nextLine;
            }
        } else {
            file >> nextLine;
        }
    }

    file.close();
}

void Board::linkCoasts(std::vector<std::vector<std::string>> coastStrings,
                       std::vector<Coast>* coasts)
{
    for (size_t i = 0; i < coastStrings.size(); ++i) {
        std::vector<std::string> coastString = coastStrings[i];
    
        for (size_t j = 2; j < coastString.size(); ++j) {
            std::string provinceName(coastString[j]);
            LandProvince* landProvince;
            int status = getLandProvince(provinceName, landProvince);

            if (status == 1) {
                std::string coastName = coastString[j + 1];
                Coast* coast;
                status = landProvince->getCoast(coastName, coast);
            
                if (status == 1) {
                    (*coasts)[i].addConnection(coast);
                    ++j;
                } else {
                    std::cout << "Error: coast linked to invalid coast.\n";
                }
            } else {
                SeaProvince* seaProvince; 
                status = getSeaProvince(provinceName, seaProvince);
        
                if (status == 1) {
                    (*coasts)[i].addSea(seaProvince);
                } else {
                    std::cout << "Error: coast linked to invalid province.\n";
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
            LandProvince* province;
            int status = getLandProvince(provinceName, province);

            if (status == 1) {
                landProvinces_[i].addNeighbor(province);
            } else {
                std::cout << "Error: land province linked to invalid province.\n";
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
            LandProvince* landProvince;
            int status = getLandProvince(provinceName, landProvince);

            if (status == 1) {
                std::string coastName = provinces[j + 1]; 
                Coast* coast;
                status = landProvince->getCoast(coastName, coast);
            
                if (status == 1) {
                    seaProvinces_[i].addCoast(coast);
                    ++j;
                } else {
                    std::cout << "Error: sea linked to invalid coast.\n";
                }
            } else {
                SeaProvince* seaProvince; 
                status = getSeaProvince(provinceName, seaProvince);
        
                if (status == 1) {
                    seaProvinces_[i].addNeighbor(seaProvince);
                } else {
                    std::cout << "Error: sea linked to invalid province.\n";
                }
            }
        } 
    }
}

void Board::addCenters(std::vector<std::string> centerStrings)
{
    for (size_t i = 0; i < centerStrings.size(); ++i) {
        std::string center = centerStrings[i];
        LandProvince* landProvince;
        int status = getLandProvince(center, landProvince);

        if (status == 1) {
            landProvince->makeCenter();
        } else {
            std::cout << "Error: supply center on invalid province.\n";
        }
    }
}
