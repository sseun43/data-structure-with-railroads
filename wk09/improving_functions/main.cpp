#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include <string>

#include "improve1.cc"
#include "improve2.cc"
#include "improve3.cc"
#include "improve4.cc"
#include "CLI11.hpp"

using namespace std;

void usage()
{
    cout << "Usage: test_improving <test_function> <test_num> [<num_of_items>]" << endl;
    cout << "  test_function: 1|2|3|4 (number of the function to test as in improve1.cc, improve2.cc, ...)" << endl;
    cout << "  test_num: number of the test performed on the function: 1|2|3. Not all have 3 tests " << endl;
    cout << "   num_of_items: number of items in the test vector (optional, default=100)" << endl;
    cout << "Examples:" << endl;
    cout << "  improving_functions 1 1    (test ascendingVector() with test case 2)" << endl;
    cout << "  improving_functions 2 2    (test minValue() with test case 2)" << endl;
    cout << "  improving_functions 3 1    (test cumulativeSums() with test case 1)" << endl;
    cout << "  improving_functions 4 1    (test randomizedThreePartQuicksort() with test case 1)" << endl;
}


int getRandom(int upper)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, upper);
    return dis(gen);
}

std::vector<int> getRandomVector(int size)
{
    auto RandomNumber = []() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(1, 100);
        return dis(gen);
    };

    std::vector<int> v(size);
    std::generate(v.begin(), v.end(), RandomNumber);
    return v;
}

void printVector(const std::vector<int>& v)
{
    cout << "[ ";
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(cout, " "));
    cout << "]" << endl << endl;
}

void printMap(const std::map<int, int>& m)
{
    for (auto it = m.begin(); it != m.end(); it++) {
        cout << "{" << it->first << " : " << it->second << "} ";
    }
    cout << endl << endl;
}


//---- Some utilities
using iter = std::vector<int>::iterator;
using RNG = std::default_random_engine;

void originalRandomizedThreePartQuicksort(iter begin, iter end, RNG rng)
{
    if(begin == end) return;
    std::shuffle(begin, end, rng);
    auto pivot = *(begin + (end - begin)/2);
    iter middle1 = std::partition(begin, end,
                                  [pivot](int val){ return val < pivot; });
    iter middle2 = std::partition(middle1, end,
                                  [pivot](int val){ return !(pivot < val); });
    randomizedThreePartQuicksort(begin, middle1, rng);
    randomizedThreePartQuicksort(middle2, end, rng);
}
void test_randomizedThreePartQuicksort(int test_id, int size) {
    cout << "Case " << test_id <<": Testing function randomizedThreePartQuicksort() with the following data:" << endl << endl;
    auto rng = std::default_random_engine {};
    switch(test_id){
    case 1:
    {
        std::vector<int> v1 = {3, 2, 4, 5};
        std::vector<int> v2 = {2, 3, 5, 4};
        printVector(v1);

        randomizedThreePartQuicksort(v1.begin(), v1.end(), rng);
        originalRandomizedThreePartQuicksort(v2.begin(), v2.end(), rng);
        if (v1 == v2)
        {
            cout << "The function is still working as expected." << endl;
        }
        else {
            cout << "FAILURE: The function is no longer working as expected. " << endl;
            cout << "The vector after randomizedThreePartQuicksort()" << endl;
            printVector(v1);
            cout << "The vector after the original randomizedThreePartQuicksort():" << endl;
            printVector(v2);
        }

        break;
    }
    case 2:
    {
        std::vector<int> bv = getRandomVector(size);
        std::vector<int> v1 = bv;
        std::vector<int> v2 = bv;
        printVector(bv);

        randomizedThreePartQuicksort(v1.begin(), v1.end(), rng);
        originalRandomizedThreePartQuicksort(v2.begin(), v2.end(), rng);
        if (v1 == v2)
        {
            cout << "The function is still working as expected." << endl;
        }
        else {
            cout << "FAILURE: The function is no longer working as expected." << endl;
            cout << "The vector after randomizedThreePartQuicksort():" << endl;
            printVector(v1);
            cout << "The vector after original randomizedThreePartQuicksort():" << endl;
            printVector(v2);
        }

        break;
    }
    default:
        cout << "ERROR: Unknown test for randomizedThreePartQuicksort: " << test_id << endl;
        cout << "       Valid values are: 1, 2." << endl;
        usage();
        break;
    }
}

std::map<int, int> originalCumulativeSums(std::vector<int> v) {
    std::map<int,int> sums;
    for (unsigned int i=0; i<v.size(); ++i) {
        if (sums.empty())
        { sums[v[i]] = v[i]; }
        else
        { sums[v[i]] = sums.at(v[i-1]) + v[i]; }
    }
    return sums;
}

void test_cumulativeSums(int test_id, int size) {
    cout << "Case " << test_id <<": Testing function cumulativeSums() with the following data:" << endl << endl;
    switch(test_id){
    case 1:
    {
        std::vector<int> v1 = {1, 2, 3, 4};
        printVector(v1);

        std::map<int, int> s1 = originalCumulativeSums(v1);
        std::map<int, int> r1 = cumulativeSums(v1);
        if(r1 == s1) {
            cout << "The map returned by the function is still correct." << endl;
        }
        else {
            cout << "FAILURE: The map returned by the function is no longer correct:" << endl;
            cout << "Correct map:" << endl;
            printMap(s1);

            cout << "The map returned by cumulativeSums():" << endl;
            printMap(r1);
        }
        break;
    }
    case 2:
    {
        std::vector<int> v2 = getRandomVector(size);
        std::map<int, int> s2 = originalCumulativeSums(v2);
        printVector(v2);
        std::map<int, int> r2 = cumulativeSums(v2);

        if (r2 == s2) {
            cout << "The map returned by the function is still correct." << endl;
        }
        else {
            cout << "FAILURE: The map returned by the function is no longer correct:" << endl;
            cout << "Correct map:" << endl;
            printMap(s2);

            cout << "The map returned  by cumulativeSums():" << endl;
            printMap(r2);
        }
        break;
    }

    default:
        cout << "ERROR: Unknown test for cumulativeSums: " << test_id << endl;
        cout << "       Valid values are: 1, 2." << endl;
        usage();
        break;
    }
}


void printMinResult(int rMin, int sMin) {
    if(rMin == sMin) {
        cout << "The function found the smallest value." << endl;
    }
    else {
        cout << "FAILURE: The function did not find the smallest value." << endl;
        cout << "The returned min value:" << rMin << endl;
    }
}

int originalMin(std::vector<int> vec){
    if ( vec.empty() ){ return 0;}
    if ( std::is_sorted(vec.begin(), vec.end()) ){
        return vec[0];
    } else {
        std::sort(vec.begin(), vec.end());
        return vec[0];
    }
}

void test_minValue(int test_id, int size) {
    cout << "Case " << test_id <<": Testing function minValue() with the following data:" << endl << endl;
    switch(test_id){
    case 1:
    {
        std::vector<int> v1 = {2, 1, 3, 4};
        int s1 = 1;
        printVector(v1);

        int r1 = minValue(v1);
        printMinResult(r1, s1);

        break;
    }
    case 2:
    {
        std::vector<int> v2 = {};
        printVector(v2);

        int r2 = minValue(v2);
        printMinResult(r2, 0);
        break;
    }
    case 3:
    {
        std::vector<int> v3 = getRandomVector(size);
        printVector(v3);

        int r3 = minValue(v3);
        int s3 = originalMin(v3);
        printMinResult(r3, s3);
        break;
    }
    default:
        cout << "ERROR: Unknown test for min: " << test_id << endl;
        cout << "       Valid values are: 1, 2, 3." << endl;
        usage();
        break;
    }
}

void test_ascendingVector(int test_id, int size) {
    cout << "Case " << test_id <<": Testing function ascendingVector() with the following data:" << endl << endl;
    switch(test_id){
    case 1:
    {
        std::vector<int> s1 = {0, 1, 2, 3};
        cout << "Size: " << s1.size() << endl;


        std::vector<int> v1 = ascendingVector(s1.size());
        if(v1 == s1) {
            cout << "The tested vector from your function is equal with the solution vector." << endl;
        }
        else {
            cout << "FAILURE: The tested vector is not equal with the solution vector." << endl;
            cout << "The vector returned by ascendingVector():" << endl;
            printVector(v1);
        }
        break;
    }
    case 2:
    {
        std::vector<int> s2 = {0, 1, 2, 3, 4, 5, 6, 7};
        cout << "Size: " << s2.size() << endl;
        std::vector<int> v2 = ascendingVector(s2.size());


        if(v2 == s2) {
            cout << "The tested vector from your function is equal with the solution vector." << endl;
        }
        else {
            cout << "FAILURE: The tested vector is not equal with the solution vector." << endl;
            cout << "The vector returned by ascendingVector():" << endl << endl;
            printVector(v2);
        }
        break;
    }
    case 3:
    {
        cout << "Size: " << size << endl;
        std::vector<int> v3 = ascendingVector(size);

        cout << "The vector returned by ascendingVector():" << endl << endl;
        printVector(v3);
        break;
    }
    default:
        cout << "ERROR: Unknown test for test_ascendingVector: " << test_id << endl;
        cout << "       Valid values are: 1, 2, 3." << endl;
        usage();
        break;
    }
}

void test(int func_id, int test_id, int size)
{
    switch (func_id)
    {
    case 1:
    {
        test_ascendingVector(test_id, size);
        break;
    }
    case 2:
    {
        test_minValue(test_id, size);
        break;
    }
    case 3:
    {
        test_cumulativeSums(test_id, size);
        break;
    }
    case 4:
    {
        test_randomizedThreePartQuicksort(test_id, size);
        break;
    }
    default:
        cout << "ERROR: Unknown function to test: " << func_id << endl;
        cout << "       Valid values are:" << endl;
        cout << "       1 (=testing ascendingVector function)" << endl;
        cout << "       2 (=testing min function)" << endl;
        cout << "       3 (=testing cumulativeSums function)" << endl;
        cout << "       4 (=testing randomizedThreePartQuicksort function)" << endl;


        usage();
        break;
    }
}

int main(int argc, char *argv[])
{

    int func_id = -1;
    int test_id = -1;
    std::vector<int> sizes = {100};
    CLI::App app{"wk09 improving functions"};
    app.add_option("test_function",func_id,"test_function: 1|2|3|4 (number of the function to test as in improve1.cc, improve2.cc, ...)")->default_val(-1);
    app.add_option("test_suite",test_id, "test suite for the function")->default_val(-1);
    app.add_option("size",sizes,"list of number(s) of items to test with (separated with space), defaults to 100, only affects the last tests for functions")->default_val(std::vector<int>{100});

    try {
        app.parse(argc,argv);
    } catch(const CLI::ParseError & e){
        app.exit(e);
        return EXIT_FAILURE;
    }
    std::map<int,int> function_to_number_of_tests = {
        {1,3},
        {2,3},
        {3,2},
        {4,2}
    };
    std::map<int,std::string> function_names = {
        {1,"ascendingVector"},
        {2,"minValue"},
        {3,"cumulativeSums"},
        {4,"randomizedThreePartQuickSort"}
    };
    if (func_id == -1 && test_id == -1) {
        // run default tests
        cout << "running default tests"<<endl;
        for (auto& function_entry : function_to_number_of_tests){
            cout << "===================================================================================="<<endl;
            cout << "Tests for function "<<function_names[function_entry.first]<<"()"<<endl<<endl;
            for(int i=1;i<=function_entry.second;++i){
                if(i==function_entry.second){
                    // run last test with two sizes
                    std::vector<int> autosizes = {100,101};
                    for (auto size : autosizes){
                        test(function_entry.first,i,size);
                        std::cout <<endl;
                    }
                }else{
                    test(function_entry.first,i,10);
                    std::cout <<endl;
                }
            }
        }
        return EXIT_SUCCESS;
    }
    else if (func_id <1 || func_id > (long long int)function_to_number_of_tests.size()){
        cout << "ERROR: Invalid function id"<<endl;
        usage();
        return EXIT_FAILURE;
    }else if (std::find_if(sizes.begin(),sizes.end(),[](auto& size_entry){return size_entry<0;})!=sizes.end()) {
        cout << "ERROR: the size should be 0 or greater"<<endl;
        usage();
        return EXIT_FAILURE;
    }

    for (auto size : sizes){
        test(func_id, test_id, size);
    }
    return EXIT_SUCCESS;
}
