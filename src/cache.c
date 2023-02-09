#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "cache.h"

/**
 * Allocate a cache entry
 */
cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length) {
    cache_entry *ce = malloc(sizeof(cache_entry));
    ce->path = path;
    ce->content_type = content_type;
    ce->content = content;
    ce->content_length = content_length;

    return ce;
}

/**
 * Deallocate a cache entry
 */
void free_entry(cache_entry *entry) {
    free(entry->path);
    free(entry->content_type);
    free(entry->content);
    free(entry);
}

/**
 * Insert a cache entry at the head of the linked list
 */
void cache_insert_head(cache *cache, cache_entry *ce) {
    // Insert at the head of the list
    if (cache->head == NULL) {
        cache->head = cache->tail = ce;
        ce->prev = ce->next = NULL;
    } else {
        cache->head->prev = ce;
        ce->next = cache->head;
        ce->prev = NULL;
        cache->head = ce;
    }
}

/**
 * Move a cache entry to the head of the list
 */
void cache_move_to_head(cache *cache, cache_entry *ce) {
    if (ce == cache->head) {
        return;
    }

    if (ce == cache->tail) {
        // We're the tail
        cache->tail = ce->prev;
        cache->tail->next = NULL;
    } else {
        // We're neither the head nor the tail
        ce->prev->next = ce->next;
        ce->next->prev = ce->prev;
    }

    ce->next = cache->head;
    cache->head->prev = ce;
    ce->prev = NULL;
    cache->head = ce;
}


/**
 * Removes the tail from the list and returns it
 * 
 * NOTE: does not deallocate the tail
 */
cache_entry *cache_remove_tail(cache *cache) {
    cache_entry *old_tail = cache->tail;

    cache->tail = old_tail->prev;
    cache->tail->next = NULL;

    cache->curr_size--;

    return old_tail;
}

/**
 * Create a new cache
 * 
 * max_size: maximum number of entries in the cache
 * hash_size: hashtable size (0 for default)
 */
cache *cache_create(int max_size, int hash_size) {
    cache *c = malloc(sizeof(cache));
    c->max_size = max_size;
    c->index->size = hash_size;

    return c;
}

void cache_free(cache *cache) {
    cache_entry *curr_entry = cache->head;

    hashtable_destroy(cache->index);

    while (curr_entry != NULL) {
        cache_entry *next_entry = curr_entry->next;

        free_entry(curr_entry);

        curr_entry = next_entry;
    }

    free(cache);
}

/**
 * Store an entry in the cache
 *
 * This will also remove the least-recently-used items as necessary.
 *
 * NOTE: doesn't check for duplicate cache entries
 */
void cache_put(cache *c, char *path, char *content_type, void *content, int content_length) {
    cache_entry *data = hashtable_get(c->index, path);

    // 1. If entry exists in hashtable, then move it to the top in the list
    if (data != NULL) {
        cache_move_to_head(c, data);
        return;
    }

    // 2. If there is not enough space, remove the tail entry from cache
    if (c->curr_size >= c->max_size) {
        cache_remove_tail(c);
    }

    // 3. Put the entry in the hashtable and add it to the list
    cache_entry *ce = alloc_entry(path, content_type, content, content_length);
    hashtable_put(c->index, path, ce);
    cache_insert_head(c, ce);
}

/**
 * Retrieve an entry from the cache
 */
cache_entry *cache_get(cache *c, char *path) {
    return hashtable_get(c->index, path);
}
