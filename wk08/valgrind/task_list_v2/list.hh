#ifndef LIST_HH
#define LIST_HH

#include <string>
#include <memory>

using namespace std;

class List {
 public:
   List();

   void insert_back(const string& insertable_task);
   bool remove_front(string& removed_task);
   bool is_empty() const;
   void print() const;

 private:
   struct List_item {
      string task;
      shared_ptr<List_item> next;
   };

   shared_ptr<List_item> first_;
   shared_ptr<List_item> last_;
};

#endif
