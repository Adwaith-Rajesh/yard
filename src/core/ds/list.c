#include "core/ds/list.h"

#define INCLUDE_LOG_IMPLEMENTATION
#include "core/utils/check.h"
#include "core/utils/log.h"

ListNode *node_init(List *list, void *data) {
    CHECK_NULL_EXIT(list, {
        LOG_ERROR("node_init: list arg cannot be NULL");
    });

    ListNode *new_node = list->allocator(sizeof(ListNode));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

List *list_init(void *(*allocator)(size_t), void (*node_data_free_fn)(ListNode *), void (*node_print_fn)(ListNode *)) {
    CHECK_NULL_EXIT(allocator, {
        LOG_ERROR("list_init: allocator arg cannot be NULL");
    });

    if (node_data_free_fn == NULL) {
        LOG_WARN("list_init: node_data_free_fn is NULL");
    }

    List *new_list = allocator(sizeof(List));
    new_list->allocator = allocator;
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

    free(node);
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
    free(list);
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
    // if you are trying to push to a NULL list, something is obviously wrong
    CHECK_NULL_EXIT(list, {
        LOG_ERROR("list_pushr: list arg is NULL");
    })

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
