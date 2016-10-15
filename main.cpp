#include <iostream>
#include "board.hpp"

int main()
{
    std::cout << "Hello World!";

    Player a("a");
    Player b("b");

    int A[] = {1, 2};
    Player B[] = {a, b};

    Board board(A, B);
}
