#ifndef YARD_DS_LIST_H
#define YARD_DS_LIST_H

#include <stddef.h>

typedef struct _node {
    void *data;
    struct _node *next;
} ListNode;

typedef struct {
    ListNode *head;
    void *(*allocator)(size_t);         // maybe one day we can implement an arena allocator
    void (*deallocator)(void *);        // maybe one day we can implement an arena allocator
    void (*node_data_free_fn)(void *);  // a function used to free the node data
    void (*node_print_fn)(ListNode *);  // a function used to print the node data
    size_t size;                        // the length of the list
} List;

// Node

// create a node with the given data
ListNode *node_init(List *list, void *data);

// print the given node
void node_print(List *list, ListNode *node);

// free the given node
void node_free(List *list, ListNode *node);

// List

// create a new list
List *list_create(void *(*allocator)(size_t), void (*deallocator)(void *), void (*node_data_free_fn)(void *), void (*node_print_fn)(ListNode *));

// free the entire list
void list_free(List *list);

// delete all the nodes from the list
void list_truncate(List *list);

// print the entire list
void list_print(List *list);

// add node to the right
List *list_pushr(List *list, void *data);

// add node to the left
List *list_pushl(List *list, void *data);

// pop the right most Node and returns the data
void *list_popr(List *list);

// pop the left most node and return the data
void *list_popl(List *list);

// delete a node if the condition matches
List *list_delete(List *list, int (*match)(ListNode *));

#endif
