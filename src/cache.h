#ifndef WEB_CACHE_H
#define WEB_CACHE_H

// Individual hash table entry
typedef struct cache_entry {
    char *path;   // Endpoint path--key to the cache
    char *content_type;
    int content_length;
    void *content;

    struct cache_entry *prev, *next; // Doubly-linked list
} cache_entry;

// A cache
typedef struct cache {
    struct hashtable *index;
    struct cache_entry *head, *tail; // Doubly-linked list
    int max_size; // Maximum number of entries
    int curr_size; // Current number of entries
} cache;

extern struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length);

extern void free_entry(struct cache_entry *entry);

extern cache *cache_create(int max_size, int hash_size);

extern void cache_free(struct cache *cache);

extern void cache_put(cache *cache, char *path, char *content_type, void *content, int content_length);

extern struct cache_entry *cache_get(cache *cache, char *path);

#endif