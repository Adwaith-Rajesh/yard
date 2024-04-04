#ifndef YARD_DS_MAP_H
#define YARD_DS_MAP_H

#include <stddef.h>

#define MAX_TABLE_SIZE 10000

// Is this is this a linked list is disguise?
// Yes, It Is.
// Did I already Implemented a LinkedList?
// Yes, I Did.
// Then why this.
// PREVENTS UNECESSARY MEMORY ALLOCATIONS, YOU IDIOT.

typedef struct _map_entry {
    // why compare strings when you can easily compare numbers
    unsigned long long hash;
    void *val;
    struct _map_entry *next;
} MapEntry;

typedef struct {
    // array of pointer to the entries
    MapEntry **entries;

    // fn used to allocate the memory for the Map, entries, and MapEntry
    void *(*allocator)(size_t);

    // fn that allows to free the val in map entry
    // used when a key is deleted
    void (*map_val_free_fn)(void *);

    // fn that allows you to change the way a MapEntry is printed
    // gives the entire map entry to print, not just the data
    void (*map_entry_print_fn)(MapEntry *);

} Map;

// returns a hash for the given key (first 64bit of SHA256)
unsigned long long hash(const char *key);

// get the index of the entry in the map
size_t map_index(const char *key);

// create a Map
// if table_size if 0 then MAX_TABLE_SIZE will be used
// allocator cannot be NULL, the rest fn pointer can be NULL
Map *map_create(size_t table_size, void *(*allocator)(size_t), void (*map_val_free_fn)(void *), void (*map_entry_print_fn)(MapEntry *));

// create a MapEntry for the given key value pair
MapEntry *map_create_kv_pair(Map *map, const char key, const char *val);

// add key value to the map
void map_set(Map *map, const char *key, const char *val);

// get value for the key
// return NULL is key does not exists.
void *map_get(Map *map, const char *key);

// delete key from the map
void map_delete(Map *map, const char *key);

// checks whether a key exists
int map_exists(Map *map, const char *key);

// call the function for each entry in the map
void map_for_each(Map *map, void (*fn)(MapEntry *));

#endif
