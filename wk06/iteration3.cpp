/**
 * iteration3.cc
 *
 * Print beginning half of a list
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include "iteration3.hh"
using namespace std;


void printHalf(const list<int>& lst)
{
    // ADD YOUR CODE HERE
    auto half = lst.size() / 2;
    auto iterator = lst.begin();
    for (int i = 0; i < half; ++i) {
        std::cout << *iterator << ' ';
        ++iterator;
    }
    std::cout << '\n';
}
