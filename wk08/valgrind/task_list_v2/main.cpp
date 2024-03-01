#include "list.hh"
#include <iostream>
#include <string>

using namespace std;


int main() {
     List task_list;
     string removed = "";

     task_list.insert_back("clean desktop");
     task_list.insert_back("wash the laundry");
     task_list.insert_back("do homeworks");

     cout << "Tasks not done:" << endl;
     task_list.print();

     task_list.remove_front(removed);
     cout << "  done: " << removed << endl;

     task_list.insert_back("wash dishes");
     task_list.insert_back("collect rubbish");

     cout << endl << "Tasks not done:" << endl;
     task_list.print();

     while ( not task_list.is_empty() ) {
        task_list.remove_front(removed);
        cout << "  done: " << removed << endl;
     }

     if ( task_list.is_empty() ) {
        cout << "All the tasks done!" << endl;
     }

     return EXIT_SUCCESS;

}
