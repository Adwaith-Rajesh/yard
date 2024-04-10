#include "core/ds/list.h"

#include "core/utils/check.h"
#include "core/utils/log.h"

ListNode *node_init(List *list, void *data) {
    ListNode *new_node = list->allocator(sizeof(ListNode));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

List *list_init(void *(*allocator)(size_t), void (*deallocator)(void *), void (*node_data_free_fn)(void *), void (*node_print_fn)(ListNode *)) {
    if (node_data_free_fn == NULL) {
        LOG_WARN("list_init: node_data_free_fn is NULL");
    }

    List *new_list = allocator(sizeof(List));
    new_list->allocator = allocator;
    new_list->deallocator = deallocator;
    new_list->node_data_free_fn = node_data_free_fn;
    new_list->node_print_fn = node_print_fn;
    new_list->head = NULL;
    return new_list;
}

void node_free(List *list, ListNode *node) {
    if (list == NULL || node == NULL) {
        return;
    }

    if (list->node_data_free_fn != NULL && node->data != NULL) {
        list->node_data_free_fn(node->data);
    };

    list->deallocator(node);
}

void list_free(List *list) {
    if (list == NULL) {
        return;
    }

    ListNode *temp;
    while (list->head != NULL) {
        temp = list->head;
        list->head = list->head->next;
        node_free(list, temp);
    }
    list->deallocator(list);
}

void node_print(List *list, ListNode *node) {
    if (list->node_print_fn == NULL) {
        printf("Node(%p){data = %p, next = %p}\n", node, node->data, node->next);
        return;
    }
    list->node_print_fn(node);
}

void list_print(List *list) {
    ListNode *temp = list->head;
    while (temp != NULL) {
        node_print(list, temp);
        temp = temp->next;
    }
}

List *list_pushr(List *list, void *data) {
    if (list->head == NULL) {
        list->head = node_init(list, data);
        return list;
    }

    ListNode *temp = list->head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = node_init(list, data);
    return list;
}

List *list_pushl(List *list, void *data) {
    ListNode *new_node = node_init(list, data);
    new_node->next = list->head;
    list->head = new_node;
    return list;
}

void *list_popr(List *list) {
    if (list->head == NULL) {
        return NULL;
    }

    ListNode *temp = list->head;
    ListNode *prev = NULL;
    while (temp->next != NULL) {
        prev = temp;
        temp = temp->next;
    }

    prev->next = NULL;
    void *data = temp->data;
    list->deallocator(temp);
    return data;
}

void *list_popl(List *list) {
    if (list->head == NULL) {
        return NULL;
    }

    ListNode *temp = list->head;
    list->head = list->head->next;
    void *data = temp->data;
    list->deallocator(temp);
    return data;
}

List *list_delete(List *list, int (*match)(ListNode *)) {
    if (list->head == NULL) {
        return list;
    }

    // check whether the head matches
    ListNode *temp = list->head;
    while (list->head != NULL && match(list->head) == 1) {
        temp = list->head;
        list->head = list->head->next;
        node_free(list, temp);
    }

    if (list->head == NULL) {
        return list;
    }

    ListNode *curr = list->head->next;
    ListNode *prev = list->head;

    while (curr != NULL) {
        if (match(curr) == 1) {
            temp = curr;
            prev->next = curr->next;
            curr = curr->next;
            node_free(list, temp);
        }
    }

    return list;
}
