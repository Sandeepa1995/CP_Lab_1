//
// Created by damitha on 3/30/19.
//
#include <iostream>
#include "linked_list.h"

struct Linked_List::Header{
    Node* next;
    Header()
            : next(nullptr) {}
};

struct Linked_List::Node{
    int value;
    Node* next;
    Node(int val)
            : value(val), next(nullptr) {}
};

Linked_List::Linked_List() : header(new Header()){}

int Linked_List::insert_node(int value) {
    Linked_List::Node* current_node_p = header->next;
    Linked_List::Node* prev_node_p = nullptr;

    while(current_node_p != nullptr && current_node_p->value < value){
        prev_node_p = current_node_p;
        current_node_p = current_node_p->next;
    }

    if (current_node_p == nullptr || current_node_p->value > value){
        Node* new_node =  new Linked_List::Node(value);
        if (prev_node_p != nullptr){
            prev_node_p->next = new_node;
            if(current_node_p!= nullptr){
                new_node->next = current_node_p;
            }
        } else {
            header->next = new_node;
        }
        return 1;
    } else {
        std::cout << "Value " << value << " is already included in the list." <<std::endl;
        return 0;
    }
}

int Linked_List::delete_node(int value) {
    Linked_List::Node* current_node_p = header->next;
    Linked_List::Node* prev_node_p = nullptr;

    while(current_node_p != nullptr && current_node_p->value < value){
        prev_node_p = current_node_p;
        current_node_p = current_node_p->next;
    }

    if (current_node_p != nullptr && current_node_p->value == value){
        prev_node_p->next = current_node_p->next;
        free(current_node_p);
        return 1;
    } else {
        std::cout << "The value " << value << " is not in the list." <<std::endl;
        return 0;
    }
}

int Linked_List::member(int value) {
    Linked_List::Node* current_node_p = header->next;
    while(current_node_p != nullptr && current_node_p->value <= value){
        if (current_node_p->value == value){
            return 1;
        }
        current_node_p = current_node_p->next;
    }
    return 0;
}

void Linked_List::print_list() {
    Linked_List::Node* current_node_p = header->next;
    while(current_node_p != nullptr){
        std::cout << current_node_p->value << std::endl;
        current_node_p = current_node_p->next;
    }
}

void Linked_List::free_list(){
    if (header->next == nullptr) return;
    header->next = nullptr;
    Linked_List::Node* current_node_p = header->next;
    Linked_List::Node* prev_node_p = nullptr;
    while(current_node_p != nullptr){
        free(prev_node_p);
        prev_node_p = current_node_p;
        current_node_p = current_node_p->next;
    }
    free(current_node_p);
}
