#include "list.hh"
#include <iostream>
#include <string>
#include <memory>

using namespace std;

List::List(): first_(nullptr),
                last_(nullptr) {
}

void List::print() const {
    shared_ptr<List_item> printable_ptr = first_;
    int running_number = 1;

    while ( printable_ptr != nullptr ) {
        cout << running_number << ". "
             << printable_ptr->task << endl;
        ++running_number;
        printable_ptr = printable_ptr->next;
    }
}

void List::insert_back(const string& insertable_task) {
    shared_ptr<List_item> new_ptr
            = make_shared<List_item>(List_item{insertable_task, nullptr});

    if ( is_empty() ) {
        first_ = new_ptr;
        last_ = new_ptr;
    } else {
        last_->next = new_ptr;
        last_ = new_ptr;
    }
}

bool List::remove_front(string& removed_task) {
    if ( is_empty() ) {
        return false;
    }

    shared_ptr<List_item> removable_ptr = first_;

    removed_task = removable_ptr->task;

    if ( first_ == last_ ) {
        first_ = nullptr;
        last_ = nullptr;
    } else {
        first_ = first_->next;
    }

    return true;
}

bool List::is_empty() const {
    return first_ == nullptr;
}
