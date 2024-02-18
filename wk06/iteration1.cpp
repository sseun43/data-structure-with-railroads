/**
 * iteration1.cc
 *
 * Print all items of a list
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include "iteration1.hh"
using namespace std;


void printAllItems(const list<int>& lst)
{
    // ADD YOUR CODE HERE
    for (auto iterator = lst.begin(); iterator != lst.end(); ++iterator) {
        std::cout << *iterator << ' ';
    }
    std::cout << '\n';
}
