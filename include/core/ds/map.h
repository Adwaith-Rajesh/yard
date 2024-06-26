#ifndef YARD_DS_MAP_H
#define YARD_DS_MAP_H

#include <stddef.h>

#define MAX_MAP_SIZE 10000

// Is this is this a linked list is disguise?
// Yes, It Is.
// Did I already Implemented a LinkedList?
// Yes, I Did.
// Then why this.
// PREVENTS UNECESSARY MEMORY ALLOCATIONS, YOU IDIOT.

typedef struct _map_entry {
    // why compare strings when you can easily compare numbers
    char *key;
    void *val;
    struct _map_entry *next;
} MapEntry;

typedef struct {
    // array of pointer to the entries
    MapEntry **entries;

    // fn used to allocate the memory for the Map, entries, and MapEntry
    void *(*allocator)(size_t);

    // fn used to deallocate memory
    void (*deallocator)(void *);

    // fn that allows to free the val in map entry
    // used when a key is deleted
    void (*map_val_free_fn)(void *);

    // fn that allows you to change the way a MapEntry is printed
    // gives the entire map entry to print, not just the data
    void (*map_entry_print_fn)(MapEntry *);

    // private var
    // the size of the map table
    size_t _map_size;

} Map;

// returns a hash for the given key (first 64bit of SHA256)
unsigned long hash(const char *key);

// create a Map
// if table_size if 0 then MAX_TABLE_SIZE will be used
// allocator cannot be NULL, the rest fn pointer can be NULL
Map *map_create(size_t table_size, void *(*allocator)(size_t), void (*deallocator)(void *), void (*map_val_free_fn)(void *), void (*map_entry_print_fn)(MapEntry *));

// create a MapEntry for the given key value pair
MapEntry *map_create_kv_pair(Map *map, const char *key, void *val);

// add key value to the map
void map_set(Map *map, const char *key, void *val);

// get value for the key
// return NULL is key does not exists.
void *map_get(Map *map, const char *key);

// delete key from the map
void map_delete(Map *map, const char *key);

// checks whether a key exists
int map_exists(Map *map, const char *key);

// call the function for each entry in the map
void map_for_each(Map *map, void (*fn)(MapEntry *, void *), void *arg);

// free the entire map
void map_free(Map *map);

#endif
