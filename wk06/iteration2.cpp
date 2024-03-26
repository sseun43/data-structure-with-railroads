/**
 * iteration2.cc
 *
 * Print every second item of a list starting from the first item
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include "iteration2.hh"
using namespace std;


void printEverySecond(const list<int>& lst)
{
    // ADD YOUR CODE HERE
    auto iterator = lst.begin();
    while (std::distance(iterator, lst.end()) > 1) {
        std::cout << *iterator << ' ';
        std::advance(iterator, 2);
    }
    if(std::distance(iterator, lst.end()) == 1) {
        std::cout << *iterator << ' ';
    }
    std::cout << '\n';
}
