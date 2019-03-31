#include <iostream>
#include <cmath>
#include "linked_list.h"

int main() {

    Linked_List l1;
    l1.insert_node(1);
    l1.insert_node(2);
    l1.insert_node(3);
    l1.insert_node(10);
    l1.print_list();
    std::cout << " " << std::endl;
    l1.insert_node(5);
    std::cout << " " << std::endl;
    std::cout << l1.member(10) << " " << l1.member(11) << std::endl;
    std::cout << " " << std::endl;
    l1.delete_node(3);
    l1.print_list();
    std::cout << " " << std::endl;
    l1.free_list();
    l1.print_list();

    double i = pow(2, 16);
    std::cout << i << std::endl;


    srand (time());


    return 0;
}