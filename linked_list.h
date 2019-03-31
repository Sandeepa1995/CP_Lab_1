//
// Created by damitha on 3/30/19.
//

#ifndef CP_LAB_1_LINKED_LIST_H
#define CP_LAB_1_LINKED_LIST_H

class Linked_List{
public:
    Linked_List();
    int insert_node(int value);
    int member(int value);
    int delete_node(int value);
    void print_list();
    int size();
    void free_list();
    static Linked_List generate_random_list(int n, int min = 0, int max = 65536){
        Linked_List new_list;
        srand(time(nullptr));

        while (new_list.size() < n){
            new_list.insert_node(rand() % (max - min) + min);
        }

        return new_list;
    }
private:
    class Node;
    class Header;
    Header* header;
};

#endif //CP_LAB_1_LINKED_LIST_H
