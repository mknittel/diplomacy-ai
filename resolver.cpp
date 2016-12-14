/**
 * resolver.cpp
 *
 * Implementation. Utilizes logical programming
 * to resolve given moves.
 */

#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include "resolver.hpp"

static std::string toLower(std::string in)
{
    int i = 0;
    char c;
    while (in[i]) {
        c = in[i];
        in[i] = putchar(tolower(c));
        ++i;
    }

    return in;
}

Resolver::Resolver(Player::MoveSet        moveSet,
                   std::vector<Province*> provinces,
                   std::vector<Player*>   players)
{
    std::ofstream resolver;
    resolver.open("resolver.pl");

    for (size_t i = 0; i < provinces.size(); ++i) {
        resolver << toLower("province(" + provinces[i]->getName() + ").\n");
    }

    for (size_t i = 0; i < 9; ++i) {
        resolver << "province(dummyprovince" << i << ").\n";
    }

    for (size_t i = 0; i < players.size(); ++i) {
        resolver << toLower("player(" + players[i]->getName() + ").\n");
    }

    for (size_t i = 0; i < 6; ++i) {
        resolver << "player(dummyplayer" << i << ").\n";
    }
    
    for (size_t i = 0; i < moveSet.convoys_.size(); ++i) {
        std::string convoysPred("");
        std::string pred = writeConvoy(moveSet.convoys_[i], convoysPred);
        resolver << toLower(convoysPred);
    }

    resolver << "convoyfleets([dummyprovince0]).\n";
    
    for (size_t i = 0; i < moveSet.convoys_.size(); ++i) {
        std::string convoysPred("");
        std::string pred = writeConvoy(moveSet.convoys_[i], convoysPred);
        resolver << toLower(pred + ".\n");
    }

    resolver << "convoy(dummyprovince1, dummyprovince2, convoyfleets([dummyprovince1]), dummyplayer0).\n";

    for (size_t i = 0; i < moveSet.moves_.size(); ++i) {
        std::string thing = toLower( writeMove(moveSet.moves_[i]) + ".\n");
        resolver << thing;
    }
        
    resolver << "move(dummyprovince3, dummyprovince4, dummyplayer1).\n";

    for (size_t i = 0; i < moveSet.supportHolds_.size(); ++i) {
        resolver << toLower(writeSupportHold(moveSet.supportHolds_[i]) + ".\n");
    }

    resolver << "supporthold(dummyprovince5, dummyplayer2, hold(dummyprovince8, dummyplayer5)).\n";

    for (size_t i = 0; i < moveSet.supportConvoys_.size(); ++i) {
        resolver << toLower(writeSupportConvoy(moveSet.supportConvoys_[i]) + ".\n");
    }

    resolver << "supportconvoy(dummyprovince6, dummyplayer3, convoy(dummyprovince1, dummyprovince2, convoyfleets([dummyprovince1]), dummyplayer0)).\n";

    for (size_t i = 0; i < moveSet.supportMoves_.size(); ++i) {
        resolver << toLower(writeSupportMove(moveSet.supportMoves_[i]) + ".\n");
    }

    resolver << "supportmove(dummyprovince7, dummyplayer4, move(dummyprovince3, dummyprovince4, dummyplayer1)).\n";

    for (size_t i = 0; i < moveSet.holds_.size(); ++i) {
        resolver << toLower(writeHold(moveSet.holds_[i]) + ".\n");
    }

    resolver << "hold(dummyprovince8, dummyplayer5).\n";

    std::ifstream framework;
    framework.open("resolverframework.pl");
    std::string nextLine("");
    while (getline(framework, nextLine)) {
        resolver << "\n";
        resolver << nextLine;
    }

    framework.close();
    resolver.close();
}

std::vector<std::string> Resolver::resolve()
{
    system("swipl -f resolver.pl <resolver.pl> -g go,halt -t 'program'");

    std::ifstream ans{"answers.txt"};
    std::vector<std::string> answers;
    std::string nextLine;
    

    while (getline(ans, nextLine)) {
        answers.push_back(nextLine);
    }

    //for (size_t i = 0; i < answers.size(); ++i) {
    //    std::cout << "Answer " << i << ": " << answers[i] << ".\n";
    //}

    return answers;
}

std::string Resolver::writeHold(Player::Hold hold)
{ 
    return "hold(" + hold.prov_ + ", " + hold.player_ + ")";
}

std::string Resolver::writeConvoy(Player::Convoy convoy, std::string& convoysPred)
{ 
    convoysPred = "convoyFleets([";

    for (size_t i = 0; i < convoy.convoy_.size(); ++i) {
        convoysPred += convoy.convoy_[i];

        if (i == convoy.convoy_.size() - 1) {
            convoysPred += "])";
        } else {
            convoysPred += ", ";
        }
    }

    std::string pred = "convoy(" + convoy.dest_ + ", " + convoy.start_ + ", "
                     + convoysPred + ", " + convoy.player_ + ")";

    convoysPred += ".\n";
    return pred;
}

std::string Resolver::writeMove(Player::Move move)
{ 
    return "move(" + move.dest_ + ", " + move.start_ + ", " + move.player_ + ")";
}

std::string Resolver::writeSupportHold(Player::SupportHold supportHold)
{ 
    std::string holdPred = writeHold(*(supportHold.hold_)); 

    std::string pred = "supportHold(" + supportHold.prov_ + ", "
                     + supportHold.player_ + ", " + holdPred + ")";
    return pred;
}

std::string Resolver::writeSupportConvoy(Player::SupportConvoy supportConvoy)
{ 
    std::string trash;
    std::string convoyPred = writeConvoy(*(supportConvoy.convoy_), trash); 

    std::string pred = "supportConvoy(" + supportConvoy.prov_ + ", "
                     + supportConvoy.player_ + ", " + convoyPred + ")";
    return pred;
}

std::string Resolver::writeSupportMove(Player::SupportMove supportMove)
{ 
    std::string movePred = writeMove(*(supportMove.move_)); 

    std::string pred = "supportMove(" + supportMove.prov_ + ", "
                     + supportMove.player_ + ", " + movePred + ")";
    return pred;
}

/*int main()
{
    Player::Hold hold;
    hold.prov_ = "Aeg";
    hold.player_ = "Italy";

    Player::Hold hold1;
    hold1.prov_ = "Den";
    hold1.player_ = "Germany";

    Player::Convoy conv;
    conv.dest_ = "Aeg";
    conv.start_ = "Naf";
    conv.player_ = "Russia";
    conv.convoy_.push_back("Mos");
    conv.convoy_.push_back("Nth");

    Player::Move mov;
    mov.dest_ = "Aeg";
    mov.start_ = "Stp";
    mov.player_ = "Britain";

    Player::SupportHold s1;
    s1.prov_ = "Par";
    s1.player_ = "France";
    s1.hold_ = &hold;

    Player::SupportConvoy s2;
    s2.prov_ = "Mar";
    s2.player_ = "Austria-Hungary";
    s2.convoy_ = &conv;
   
    Player::SupportMove s3;
    s3.prov_ = "War";
    s3.player_ = "Turkey";
    s3.move_ = &mov;
   
    Player::SupportMove s4;
    s4.prov_ = "Rom";
    s4.player_ = "Turkey";
    s4.move_ = &mov;
    
    Province r("Aeg");
    Province r1("Den");
    Province r2("Mos");
    Province r3("Nth");
    Province r4("Rom");
    Province r5("Stp");
    Province r6("Par");
    Province r7("Mar");
    Province r8("War");
    Province r9("Naf");

    Player p("Germany");
    Player p1("Turkey");
    Player p2("Austria-Hungary");
    Player p3("Britain");
    Player p4("Italy");
    Player p5("Russia");
    Player p6("France");

    Player::MoveSet moveSet;

    moveSet.holds_.push_back(hold);
    moveSet.holds_.push_back(hold1);

    moveSet.convoys_.push_back(conv);

    moveSet.moves_.push_back(mov);

    for (size_t i = 0; i < moveSet.moves_.size(); ++i) {
        std::cout << "move " << i << " is " << moveSet.moves_[i].player_ << moveSet.moves_[i].dest_ << "/n";
    }

    moveSet.supportHolds_.push_back(s1);

    moveSet.supportConvoys_.push_back(s2);

    moveSet.supportMoves_.push_back(s3);
    moveSet.supportMoves_.push_back(s4);

    std::vector<Province*> provinces;
    std::vector<Player*> players;

    provinces.push_back(&r);
    provinces.push_back(&r1);
    provinces.push_back(&r2);
    provinces.push_back(&r3);
    provinces.push_back(&r4);
    provinces.push_back(&r5);
    provinces.push_back(&r6);
    provinces.push_back(&r7);
    provinces.push_back(&r8);
    provinces.push_back(&r9);

    players.push_back(&p);
    players.push_back(&p1);
    players.push_back(&p2);
    players.push_back(&p3);
    players.push_back(&p4);
    players.push_back(&p5);
    players.push_back(&p6);
    
    Resolver resolver(moveSet, provinces, players);
    std::vector<std::string> resolutions = resolver.resolve();

    for (size_t i = 0; i < resolutions.size(); ++i) {
        std::cout << resolutions[i] << "\n";
    }

    return 0;
}*/
