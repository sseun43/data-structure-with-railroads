#include <iostream>
#include <random>
#include <list>
#include "CLI11.hpp"
#include "iteration1.hh"
#include "iteration2.hh"
#include "iteration3.hh"
#include "iteration4.hh"


struct testablefunction{
    void (*fun)(const std::list<int>&);
    std::string name = "NO_FUNCTION_NAME";
};

const std::vector<testablefunction> FUNCTIONS = {
    {&printAllItems,"printAllItems"},
    {&printEverySecond,"printEverySecond"},
    {&printHalf,"printHalf"},
    {&printReverse,"printReverse"}
};

const int NUMBER_OF_FUNCTIONS = FUNCTIONS.size();

int getRandom(int upper)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, upper);
    return dis(gen);
}

std::list<int> getRandomList(std::size_t size)
{
    std::set<int> s;
    while (s.size() < size)
    {
        int value = getRandom(size * 2);
        s.insert(value);
    }

    std::list<int> lst(s.begin(), s.end());
    return lst;
}

void printData(const std::vector<int>& data)
{
    // Using vector and regular indexing and not iterators on purpose here
    // to avoid revealing how to loop with iterators
    for (std::size_t i = 0; i < data.size(); ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl << std::endl;
}

void test_function(int selected_function, int number_of_items, std::list<int>& data){
    using std::cout, std::endl;
    auto testfunct = FUNCTIONS[selected_function - 1];
    cout << "Testing function " << testfunct.name << "() with the following data:" << endl << endl;
    std::list<int> lst = getRandomList(number_of_items);
    if (data.size() > 0) {
        lst=std::move(data);
    }else{
        lst = getRandomList(number_of_items);
    }

    std::vector<int> vect(lst.begin(), lst.end());
    printData(vect);
    cout << "Your function printed the following values:" << endl << endl;
    testfunct.fun(lst);
}


int main(int argc, char *argv[])
{
    int selected_function = std::numeric_limits<int>::min();
    int number_of_items = -1;
    std::list<int> data = {};
    CLI::App app{"Week 06 iteration"};

    app.add_option("test_function", selected_function, "1|2|3|4 (number of the function to test as in iteration1.cc, iteration2.cc, ...)")->default_val(std::numeric_limits<int>::min());
    auto par_n = app.add_option("num_of_items", number_of_items, "number of items in the test set (optional, default=10)")->default_val(10);
    auto par_data = app.add_option("--data", data, "data to manually be used for testing")->type_size(1);
    par_n->excludes(par_data);
    par_data->excludes(par_n);

    try {
        app.parse(argc,argv);
    } catch (const CLI::ParseError & e) {
        app.exit(e);
        return EXIT_FAILURE;
    }
    if(selected_function==std::numeric_limits<int>::min()){
        // run all the tests with default values if no function was specified
        std::vector<int> n_values = {10,11,101,500};
        for(int funcnumber = 1; funcnumber<=NUMBER_OF_FUNCTIONS; ++funcnumber){
            std::cout << "=============================================="<<std::endl;
            std::cout << "Testing function "<< FUNCTIONS[funcnumber-1].name<<" with N values {";
            for (auto n : n_values){
                std::cout << n << " ";
            }
            std::cout << "} "<< std::endl;
            std::cout << "=============================================="<<std::endl<<std::endl;
            for(auto n: n_values){
                std::list<int>d;
                test_function(funcnumber,n,d);
            }
        }
        return EXIT_SUCCESS;
    }

    using namespace std;

    if (selected_function<1 || selected_function > NUMBER_OF_FUNCTIONS) {
        cout << "No such test function" << endl;
        return EXIT_FAILURE;
    }
    test_function(selected_function,number_of_items,data);



    return EXIT_SUCCESS;
}
