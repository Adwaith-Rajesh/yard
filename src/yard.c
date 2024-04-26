#include "yard.h"

#include <stdlib.h>

#include "core/ds/list.h"
#include "core/ds/map.h"

// helper functions
void _container_print_list(ListNode *node) {
    if (node == NULL) {
        return;
    }
    container_print((Container *)node->data);
}

void _container_print_map(MapEntry *entry) {
    if (entry == NULL) {
        return;
    }
    container_print((Container *)entry->val);
}

void _container_free(void *data) {
    if (data == NULL) {
        return;
    }
    container_free((Container *)data);
}

// end helper

static void _free_list_from_map(void *data) {
    if (data == NULL) {
        return;
    }
    list_free((List *)data);
}

static void _free_map_from_map(void *data) {
    if (data == NULL) {
        return;
    }
    map_free((Map *)data);
}

YardMasterCtx *mctx_create(void *(*allocator)(size_t), void (*deallocator)(void *), void *(*reallocator)(void *, size_t)) {
    YardMasterCtx *new_mctx = allocator(sizeof(YardMasterCtx));

    new_mctx->allocator = allocator;
    new_mctx->deallocator = deallocator;
    new_mctx->reallocator = reallocator;

    new_mctx->_default_map = MAP_FROM_MCTX(new_mctx);
    new_mctx->_default_list = LIST_FROM_MCTX(new_mctx);

    new_mctx->_user_list = map_create(0, allocator, deallocator, _free_list_from_map, NULL);
    new_mctx->_user_maps = map_create(0, allocator, deallocator, _free_map_from_map, NULL);

    return new_mctx;
}

void mctx_free(YardMasterCtx *mctx) {
    map_free(mctx->_default_map);
    map_free(mctx->_user_maps);
    list_free(mctx->_default_list);
    map_free(mctx->_user_list);

    mctx->deallocator(mctx);
}
