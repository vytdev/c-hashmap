#include "hashmap.h"
#include <stdio.h>
#include <stdarg.h>

static inline void pLog(const char *msg, ...) {
  fprintf(stderr, "test: ");
  va_list list;
  va_start(list, msg);
  vfprintf(stderr, msg, list);
  va_end(list);
}

int main(void) {
  HashMap map;
  if (!hashMap_init(&map, 10, .75f)) {
    pLog("init failed\n");
    return 1;
  }

  // insert {hello=world}
  if (!hashMap_insert(&map, "hello", "world")) {
    pLog("insert failed for key: hello\n");
    hashMap_destroy(&map);
    return 1;
  }

  // insert {hi=coder}
  if (!hashMap_insert(&map, "hi", "coder")) {
    pLog("insert failed for key: hi");
    hashMap_destroy(&map);
    return 1;
  }

  char *val;

  // lookup hello=?
  if (!hashMap_lookup(&map, "hello", (void**)&val)) {
    pLog("lookup failed: key not found 'hello'\n");
    hashMap_destroy(&map);
    return 1;
  }
  pLog("hello=%s\n", val);

  // lookup hi=?
  if (!hashMap_lookup(&map, "hi", (void**)&val)) {
    pLog("lookup failed: key not found 'hi'\n");
    hashMap_destroy(&map);
    return 1;
  }
  pLog("hi=%s\n", val);

  // lookup unknown=? (expected to fail)
  if (hashMap_lookup(&map, "unknown", NULL)) {
    pLog("lookup succeeded (but expected to fail): key found 'unknown'\n");
    hashMap_destroy(&map);
    return 1;
  }

  // release the hashmap
  hashMap_destroy(&map);
  pLog("all tests succeeded\n");
  return 0;
}
