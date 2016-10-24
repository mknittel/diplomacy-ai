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

Resolver::Resolver(std::vector<Resolver::Hold>          holds,
                   std::vector<Resolver::Convoy>        convoys,
                   std::vector<Resolver::Move>          moves,
                   std::vector<Resolver::SupportHold>   supportHolds,
                   std::vector<Resolver::SupportConvoy> supportConvoys,
                   std::vector<Resolver::SupportMove>   supportMoves,
                   std::vector<Province*>               provinces,
                   std::vector<Player*>                 players)
{
    std::ofstream resolver;
    resolver.open("resolver.pl");

    // TODO: add provinces, players, also consider coast AFTER prolog
    for (size_t i = 0; i < provinces.size(); ++i) {
        resolver << toLower("province(" + provinces[i]->getName() + ").\n");
    }

    for (size_t i = 0; i < players.size(); ++i) {
        resolver << toLower("player(" + players[i]->getName() + ").\n");
    }
    
    for (size_t i = 0; i < convoys.size(); ++i) {
        std::string convoysPred("");
        
        std::string pred = writeConvoy(convoys[i], convoysPred);

        resolver << toLower(convoysPred);
        resolver << toLower(pred + ".\n");
    }

    for (size_t i = 0; i < moves.size(); ++i) {
        resolver <<toLower( writeMove(moves[i]) + ".\n");
    }

    for (size_t i = 0; i < supportHolds.size(); ++i) {
        resolver << toLower(writeSupportHold(supportHolds[i]) + ".\n");
    }

    for (size_t i = 0; i < supportConvoys.size(); ++i) {
        resolver << toLower(writeSupportConvoy(supportConvoys[i]) + ".\n");
    }

    for (size_t i = 0; i < supportMoves.size(); ++i) {
        resolver << toLower(writeSupportMove(supportMoves[i]) + ".\n");
    }

    for (size_t i = 0; i < holds.size(); ++i) {
        resolver << toLower(writeHold(holds[i]) + ".\n");
    }


    std::ifstream framework;
    framework.open("resolverframework.pl");
    std::string nextLine("");
    while (getline(framework, nextLine)) {
        resolver << "\n";
        resolver << nextLine;
    }

    framework.close();
    resolver.close();

    system("swipl -f resolver.pl <resolver.pl> -g go,halt -t 'program'");

    std::ifstream ans{"answers.txt"};
    std::vector<std::string> answers;
    
    while (getline(ans, nextLine)) {
        answers.push_back(nextLine);
    }

    for (size_t i = 0; i < answers.size(); ++i) {
        std::cout << "Answer " << i << ": " << answers[i] << ".\n";
    }
}

std::string Resolver::writeHold(Resolver::Hold hold)
{ 
    return "hold(" + hold.prov_ + ", " + hold.player_ + ")";
}

std::string Resolver::writeConvoy(Resolver::Convoy convoy, std::string& convoysPred)
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

std::string Resolver::writeMove(Resolver::Move move)
{ 
    return "move(" + move.dest_ + ", " + move.start_ + ", " + move.player_ + ")";
}

std::string Resolver::writeSupportHold(Resolver::SupportHold supportHold)
{ 
    std::string holdPred = writeHold(*(supportHold.hold_)); 

    std::string pred = "supportHold(" + supportHold.prov_ + ", "
                     + supportHold.player_ + ", " + holdPred + ")";
    return pred;
}

std::string Resolver::writeSupportConvoy(Resolver::SupportConvoy supportConvoy)
{ 
    std::string trash;
    std::string convoyPred = writeConvoy(*(supportConvoy.convoy_), trash); 

    std::string pred = "supportConvoy(" + supportConvoy.prov_ + ", "
                     + supportConvoy.player_ + ", " + convoyPred + ")";
    return pred;
}

std::string Resolver::writeSupportMove(Resolver::SupportMove supportMove)
{ 
    std::string movePred = writeMove(*(supportMove.move_)); 

    std::string pred = "supportMove(" + supportMove.prov_ + ", "
                     + supportMove.player_ + ", " + movePred + ")";
    return pred;
}

int main()
{
    Resolver::Hold hold;
    hold.prov_ = "Aeg";
    hold.player_ = "Italy";

    Resolver::Hold hold1;
    hold1.prov_ = "Den";
    hold1.player_ = "Germany";

    Resolver::Convoy conv;
    conv.dest_ = "Aeg";
    conv.start_ = "Naf";
    conv.player_ = "Russia";
    conv.convoy_.push_back("Mos");
    conv.convoy_.push_back("Nth");

    Resolver::Move mov;
    mov.dest_ = "Aeg";
    mov.start_ = "Stp";
    mov.player_ = "Britain";

    Resolver::SupportHold s1;
    s1.prov_ = "Par";
    s1.player_ = "France";
    s1.hold_ = &hold;

    Resolver::SupportConvoy s2;
    s2.prov_ = "Mar";
    s2.player_ = "Austria-Hungary";
    s2.convoy_ = &conv;
   
    Resolver::SupportMove s3;
    s3.prov_ = "War";
    s3.player_ = "Turkey";
    s3.move_ = &mov;
   
    Resolver::SupportMove s4;
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

    std::vector<Resolver::Hold> holds;
    holds.push_back(hold);
    holds.push_back(hold1);

    std::vector<Resolver::Convoy> convoys;
    convoys.push_back(conv);

    std::vector<Resolver::Move> moves;
    moves.push_back(mov);

    std::vector<Resolver::SupportHold> supportHolds;
    supportHolds.push_back(s1);

    std::vector<Resolver::SupportConvoy> supportConvoys;
    supportConvoys.push_back(s2);

    std::vector<Resolver::SupportMove> supportMoves;
    supportMoves.push_back(s3);
    supportMoves.push_back(s4);

    std::vector<Province*> provinces;
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
    

    std::vector<Player*> players;
    players.push_back(&p);
    players.push_back(&p1);
    players.push_back(&p2);
    players.push_back(&p3);
    players.push_back(&p4);
    players.push_back(&p5);
    players.push_back(&p6);

    Resolver(holds, convoys, moves, supportHolds, supportConvoys, supportMoves, provinces, players);
    
    return 0;
}
