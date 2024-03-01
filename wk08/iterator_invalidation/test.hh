#ifndef TEST_HH
#define TEST_HH

#include <vector>
#include <map>
#include <string>

const int NOT_FOUND = -1;


/**
 * @brief duplicates even numbers in the vector, removes uneven numbers. Example: {1, 2, 3, 4} -> {2, 2, 4, 4}
 *
 * @param vec vector to be handled
 */
void duplicateEvenRemoveUneven(std::vector<int>& vec);


/**
 * @brief creates a vector with ascending numbers from 0 - n-1
 *
 * @param n the size of the vector to be created
 */
std::vector<int> ascendingVector(int n);

/**
 * @brief Erases every second item from the vector. Example: {1, 2, 3, 4} -> {1, 3}
 *
 * @param vec vector where every second item is erased.
 */
void eraseEverySecond(std::vector<int>& vec);

#endif // TEST_HH
