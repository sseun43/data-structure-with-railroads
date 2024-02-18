/**
 * iteration4.cc
 *
 * Print all items of a list in a reverse order
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include "iteration4.hh"
using namespace std;


void printReverse(const list<int>& lst)
{
    // ADD YOUR CODE HERE
    for (auto iterator = lst.rbegin(); iterator != lst.rend(); ++iterator) {
        std::cout << *iterator << ' ';
    }
    std::cout << '\n';
}
