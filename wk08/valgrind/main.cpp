#include <iostream>

using namespace std;


int main() {
    int number1;
    int number2 = 111;
    int *ptr1 = new int;
    int *ptr2 = new int(222);

    cout << number1 << " "
         << number2 << " "
         << *ptr1 << " "
         << *ptr2 << endl;

    delete ptr1;

    *ptr1 = 333;
}
