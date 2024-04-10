#include "map.h"

#include <string.h>

#include "core/utils/check.h"
#include "core/utils/log.h"

Map *map_create(size_t table_size,
                void *(*allocator)(size_t),
                void (*deallocator)(void *),
                void (*map_val_free_fn)(void *),
                void (*map_entry_print_fn)(MapEntry *)) {
    CHECK_NULL_EXIT(allocator, {
        LOG_ERROR("map_create: allocator cannot be null");
    });

    CHECK_NULL_EXIT(deallocator, {
        LOG_ERROR("map_create: dellocator cannot be null");
    });

    size_t _t_size = (table_size != 0) ? table_size : MAX_TABLE_SIZE;

    Map *new_map = allocator(sizeof(Map));
    new_map->allocator = allocator;
    new_map->deallocator;
    new_map->map_val_free_fn = map_val_free_fn;
    new_map->map_entry_print_fn = map_entry_print_fn;

    new_map->entries = allocator(sizeof(MapEntry *) * _t_size);

    // set all entries NULL
    for (size_t i = 0; i < _t_size; i++) {
        new_map->entries[i] = NULL;
    }

    return new_map;
}

// implementation stolen from
// http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(const char *key) {
    unsigned long hash = 5381;
    int c;

    while (c = *key++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
