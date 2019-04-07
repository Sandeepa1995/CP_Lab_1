//
// Created by damitha on 3/30/19.
//

#ifndef CP_LAB_1_LINKED_LIST_H
#define CP_LAB_1_LINKED_LIST_H

#include <iostream>
#include <time.h>

struct timespec ts;

// Linked List class
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
        clock_gettime(CLOCK_MONOTONIC, &ts);
        srand(time(nullptr)*pow(10,9) + (time_t)ts.tv_nsec);

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

// Block printing of messages
bool BLOCK_PRINT = true;

// Header
struct Linked_List::Header{
    Node* next;
    Header()
            : next(nullptr) {}
};


// Node
struct Linked_List::Node{
    int value;
    Node* next;
    Node(int val)
            : value(val), next(nullptr) {}
};

// Init Linked List
Linked_List::Linked_List() : header(new Header()){}

// Insert value to Linked List
int Linked_List::insert_node(int value) {
    // Variables to iterate the list
    Linked_List::Node* current_node_p = header->next;
    Linked_List::Node* prev_node_p = nullptr;

    // Iterate current and previous nodes till the value is exceeded
    while(current_node_p != nullptr && current_node_p->value < value){
        prev_node_p = current_node_p;
        current_node_p = current_node_p->next;
    }

    // Add to list if value is lower than the value of the current node
    if (current_node_p == nullptr || current_node_p->value > value){
        // New node
        Node* new_node =  new Linked_List::Node(value);
        // Check if previous node is null. (To handle inserting to the start of the list vs at any other point)
        if (prev_node_p != nullptr){
            prev_node_p->next = new_node;
            // If not at the end of the list
            if(current_node_p!= nullptr){
                new_node->next = current_node_p;
            }
        } else {
            header->next = new_node;
            if(current_node_p!= nullptr){
                new_node->next = current_node_p;
            }
        }
        // Return 1 as success
        return 1;
    } else {
        // Value is already in the list
        if (!BLOCK_PRINT){
            std::cout << "Value " << value << " is already included in the list." <<std::endl;
        }
        // Return 0 as faliure
        return 0;
    }
}

// Search if a value is already in the list
int Linked_List::member(int value) {
    // Iterate through the list
    Linked_List::Node* current_node_p = header->next;
    while(current_node_p != nullptr && current_node_p->value <= value){
        if (current_node_p->value == value){
            // Return 1 if in the list
            return 1;
        }
        current_node_p = current_node_p->next;
    }
    // Return 0 if not in the list
    return 0;
}

// Delete value in list
int Linked_List::delete_node(int value) {
    // Variables to iterate the list
    Linked_List::Node* current_node_p = header->next;
    Linked_List::Node* prev_node_p = nullptr;

    // Iterate current and previous nodes till the value is exceeded
    while(current_node_p != nullptr && current_node_p->value < value){
        prev_node_p = current_node_p;
        current_node_p = current_node_p->next;
    }

    // Delete from the list if the value is found
    if (current_node_p != nullptr && current_node_p->value == value){
        // Handle deletion of next of header and any other
        if (prev_node_p!= nullptr){
            prev_node_p->next = current_node_p->next;
        } else {
            header->next = current_node_p->next;
        }
        free(current_node_p);
        return 1;
    } else {
        if (!BLOCK_PRINT){
            std::cout << "The value " << value << " is not in the list." <<std::endl;
        }
        return 0;
    }
}

// Print all the values in the list, line-by-line
void Linked_List::print_list() {
    // Variable to iterate the list
    Linked_List::Node* current_node_p = header->next;
    while(current_node_p != nullptr){
        std::cout << current_node_p->value << std::endl;
        current_node_p = current_node_p->next;
    }
}

// Get the size of the list (Number of values stored in the list)
int Linked_List::size() {
    int length = 0;
    Linked_List::Node* current_node_p = header->next;
    while(current_node_p != nullptr){
        length += 1;
        current_node_p = current_node_p->next;
    }
    return length;
}

// Free the list (Delete all nodes)
void Linked_List::free_list(){
    // If empty list
    if (header->next == nullptr) return;
    // Variables to iterate the list
    Linked_List::Node* current_node_p = header->next;
    Linked_List::Node* prev_node_p = nullptr;
    // Free element by element
    while(current_node_p != nullptr){
        free(prev_node_p);
        prev_node_p = current_node_p;
        current_node_p = current_node_p->next;
    }
    header->next = nullptr;
    free(current_node_p);
}

#endif //CP_LAB_1_LINKED_LIST_H
