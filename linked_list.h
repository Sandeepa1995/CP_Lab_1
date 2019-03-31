//
// Created by damitha on 3/30/19.
//

#ifndef CP_LAB_1_LINKED_LIST_H
#define CP_LAB_1_LINKED_LIST_H

class Linked_List{
public:
    Linked_List();
    int insert_node(int value);
    void print_list();
    int member(int value);
    int delete_node(int value);
    void free_list();
private:
    class Node;
    class Header;
    Header* header;
};

#endif //CP_LAB_1_LINKED_LIST_H
