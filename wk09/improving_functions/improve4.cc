#include <iterator>
#include <vector>
#include <algorithm>
#include <random>


//---- Some utilities
using iter = std::vector<int>::iterator;
using RNG = std::default_random_engine;

/**
 * @brief Sorts a given vector using a 3 part randomized quicksort algorithm.
 *        Randomization is used to avoid worst case (where the pivot value is
 *        chosen poorly).
 *
 * HINT: The way randomization is implemented can be changed. Shuffle may not
 *       be the only or the best way to implement randomization.
 *
 * @param begin an iterator that points to the beginning part of the sequence where the quicksort is performed.
 * @param end an iterator that points to the end part of the sequence where the quicksort is performed
 * @param rng the random number generator that can be used
 */
void randomizedThreePartQuicksort(iter begin, iter end, RNG& rng)
{
    if (begin == end) return;
    std::shuffle(begin, end, rng);
    auto pivot = *(begin + (end - begin)/2);
    iter middle1 = std::partition(begin, end,
        [pivot](int val){ return val < pivot; });
    iter middle2 = std::partition(middle1, end,
        [pivot](int val){ return !(pivot < val); });
    randomizedThreePartQuicksort(begin, middle1, rng);
    randomizedThreePartQuicksort(middle2, end, rng);
}
