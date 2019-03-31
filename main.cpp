#include <iostream>
#include <cmath>
#include <time.h>
#include "linked_list.h"

int main() {

   Linked_List l1 = Linked_List::generate_random_list(1000);

   l1.print_list();
   std::cout << l1.size() <<std::endl;

    return 0;
}