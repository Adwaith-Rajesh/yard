#include "map.h"

#include <string.h>
#include <strings.h>

#include "core/utils/check.h"
#include "core/utils/log.h"

#define MAP_INDEX(map, key) (hash(key) % map->_map_size)

Map *map_create(size_t table_size,
                void *(*allocator)(size_t),
                void (*deallocator)(void *),
                void (*map_val_free_fn)(void *),
                void (*map_entry_print_fn)(MapEntry *)) {
    size_t _t_size = (table_size != 0) ? table_size : MAX_MAP_SIZE;

    Map *new_map = allocator(sizeof(Map));
    new_map->allocator = allocator;
    new_map->deallocator = deallocator;
    new_map->map_val_free_fn = map_val_free_fn;
    new_map->map_entry_print_fn = map_entry_print_fn;
    new_map->_map_size = _t_size;
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

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

MapEntry *map_create_kv_pair(Map *map, const char *key, void *val) {
    MapEntry *new_map_entry = map->allocator(sizeof(MapEntry));

    size_t key_len = strlen(key) + 1;

    new_map_entry->key = map->allocator(key_len + 1);
    bzero(new_map_entry->key, key_len + 1);
    strncpy(new_map_entry->key, key, key_len);

    new_map_entry->val = val;
    new_map_entry->next = NULL;
    return new_map_entry;
}

void map_set(Map *map, const char *key, void *val) {
    size_t index = MAP_INDEX(map, key);

    if (map->entries[index] == NULL) {
        map->entries[index] = map_create_kv_pair(map, key, val);
        return;
    }

    MapEntry *temp = map->entries[index];
    MapEntry *prev = NULL;
    while (temp != NULL) {
        if (strncmp(temp->key, key, strlen(key)) == 0) {
            map->map_val_free_fn(temp->val);
            temp->val = val;
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    prev->next = map_create_kv_pair(map, key, val);
}

void *map_get(Map *map, const char *key) {
    size_t index = MAP_INDEX(map, key);

    if (map->entries[index] == NULL) {
        return NULL;
    }

    MapEntry *temp = map->entries[index];

    while (temp != NULL && strncmp(temp->key, key, strlen(key)) != 0) {
        temp = temp->next;
    }

    if (temp != NULL) {
        return temp->val;
    }
    return NULL;
}

void map_delete(Map *map, const char *key) {
    size_t index = MAP_INDEX(map, key);

    if (map->entries[index] == NULL) {
        return;
    }

    MapEntry *temp = map->entries[index];
    MapEntry *prev = NULL;
    while (temp != NULL && strncmp(temp->key, key, strlen(key))) {
        temp = temp->next;
    }

    // first node
    if (prev == NULL) {
        map->entries[index] = map->entries[index]->next;
        map->map_val_free_fn(temp->val);
        map->deallocator(temp);
        return;
    }

    prev->next = temp->next;
    map->map_val_free_fn(temp->val);
    map->deallocator(temp);
}

void map_free(Map *map) {
    printf("map->entries %p\n", map->entries[0]);

    for (size_t i = 0; i < map->_map_size; ++i) {
        if (map->entries[i] == NULL) {
            continue;
        }

        MapEntry *temp = NULL;
        while (map->entries[i] != NULL) {
            temp = map->entries[i];
            map->entries[i] = map->entries[i]->next;
            map->map_val_free_fn(temp->val);
            map->deallocator(temp->key);
            map->deallocator(temp);
        }
    }
    map->deallocator(map->entries);

    // this is the weirdest line of code i've ever written
    map->deallocator(map);
}
