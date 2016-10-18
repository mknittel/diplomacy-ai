#include <iostream>
#include "board.hpp"

int main()
{
    std::cout << "Hello World!";

/*    Player a("a");
    Player b("b");

    Province p("a", true, a);

    std::vector<Province*> A = {&p, &p};
    std::vector<Player*> B = {&a,&b};
*/
    Board board;

    board.uploadBoard("europe.txt");
}
