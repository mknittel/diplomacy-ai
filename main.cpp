#include <iostream>
#include "game.hpp"

int main()
{
    std::cout << "Hello World!\n";

    Player a("a");
    Player b("b");

    Province p("a");

    std::vector<Province*> A = {&p, &p};
    std::vector<Player*> B = {&a,&b};

    Board board;
    board.uploadBoard("europe.txt");

    //Game("europe.txt");

    return 0;
}
