#include "hashmap.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static inline uint32_t fnv_1a_32(char *data) {
  uint32_t hash = 0x811c9dc5; // offset basis

  while (*data) {
    hash ^= *data;
    hash *= 0x01000193;       // prime
    data++;
  }

  return hash;
}


bool hashMap_init(HashMap *map, size_t initCapacity, float loadFactor) {
  if (!map)
    return false;

  if (initCapacity == 0)
    initCapacity = 2;

  map->items = (HashMapItem**)malloc(sizeof(HashMapItem*) * initCapacity);
  if (!map->items)
    return false;
  memset(map->items, 0, sizeof(HashMapItem*) * initCapacity);

  map->capacity   = initCapacity;
  map->length     = 0;
  map->loadFactor = loadFactor;

  return true;
}

static inline void hashMap__freeItemsHelper(HashMapItem *item) {
  if (!item) return;
  hashMap__freeItemsHelper(item->next);
  if (item->key) free(item->key);
  free(item);
}

bool hashMap_destroy(HashMap *map) {
  if (!map)
    return false;

  if (map->items) {
    for (size_t i = 0; i < map->capacity; i++)
      hashMap__freeItemsHelper(map->items[i]);
    free(map->items);
  }

  map->items      = NULL;
  map->capacity   = 0;
  map->length     = 0;
  map->loadFactor = 0.0f;

  return true;
}

bool hashMap_resize(HashMap *map, size_t newCapacity) {
  if (!map || newCapacity == map->capacity)
    return false;

  HashMapItem **newList = (HashMapItem**)malloc(sizeof(HashMapItem*) * newCapacity);
  if (!newList)
    return false;
  memset(newList, 0, sizeof(HashMapItem*) * newCapacity);

  for (size_t i = 0; i < map->capacity; i++) {
    HashMapItem *curr = map->items[i];
    while (curr) {
      HashMapItem *next = curr->next;
      uint32_t hash = fnv_1a_32(curr->key);
      size_t idx = hash % newCapacity;
      curr->next = newList[idx];
      newList[idx] = curr;
      curr = next;
    }
  }

  free(map->items);
  map->items = newList;
  map->capacity = newCapacity;

  return true;
}

bool hashMap_insert(HashMap *map, char *key, void *val) {
  if (!map || !key)
    return false;

  if (hashMap_lookup(map, key, NULL))
    return false;

  if (map->length >= map->loadFactor * map->capacity &&
      !hashMap_resize(map, map->capacity * 2))
    return false;

  HashMapItem *item = (HashMapItem*)malloc(sizeof(HashMapItem));
  if (!item)
    return false;

  size_t keyLen = strlen(key);
  char *keyCopy = (char*)malloc(keyLen + 1);
  if (!keyCopy) {
    free(item);
    return false;
  }
  memcpy(keyCopy, key, keyLen);
  keyCopy[keyLen] = '\0';

  uint32_t hash = fnv_1a_32(key);
  size_t idx = hash % map->capacity;

  item->next = map->items[idx];
  item->key  = keyCopy;
  item->data = val;
  map->items[idx] = item;
  map->length++;

  return true;
}

bool hashMap_delete(HashMap *map, char *key) {
  if (!map || !key)
    return false;

  uint32_t hash = fnv_1a_32(key);
  size_t idx = hash % map->capacity;

  HashMapItem *prev = NULL;
  HashMapItem *curr = map->items[idx];
  while (curr) {
    if (strcmp(curr->key, key) != 0) {
      prev = curr;
      curr = curr->next;
      continue;
    }

    if (curr == map->items[idx])
      map->items[idx] = curr->next;
    if (prev)
      prev->next = curr->next;
    free(curr->key);
    free(curr);
    map->length--;

    return true;
  }

  return false;
}

bool hashMap_lookup(HashMap *map, char *key, void **out) {
  if (!map || !key)
    return false;

  uint32_t hash = fnv_1a_32(key);
  size_t idx = hash % map->capacity;

  HashMapItem *curr = map->items[idx];
  while (curr) {
    if (strcmp(curr->key, key) != 0) {
      curr = curr->next;
      continue;
    }

    if (out) *out = curr->data;
    return true;
  }

  return false;
}
