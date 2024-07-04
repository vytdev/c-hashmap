#ifndef VYT_HASHMAP_H_
#define VYT_HASHMAP_H_
#include <stddef.h>
#include <stdbool.h>

typedef struct HashMapItem {
  struct HashMapItem *next;
  char          *key;
  void          *data;
} HashMapItem;

typedef struct HashMap {
  size_t        capacity;
  size_t        length;
  HashMapItem   **items;
  float         loadFactor;
} HashMap;

bool hashMap_init(HashMap *map, size_t initCapacity, float loadFactor);

bool hashMap_destroy(HashMap *map);

bool hashMap_resize(HashMap *map, size_t newCapacity);

bool hashMap_insert(HashMap *map, char *key, void *val);

bool hashMap_delete(HashMap *map, char *key);

bool hashMap_lookup(HashMap *map, char *key, void **out);

#endif // VYT_HASHMAP_H_
