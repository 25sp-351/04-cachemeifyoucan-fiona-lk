// Fiona Keaney
// fionakeaney@csu.fullerton.edu


#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

struct cache_struct {
    cache_entry* entries;
    int capacity;
    int size;
    int insert_pos;    // Position for next insertion
    int hits;          // Cache hit counter
    int misses;        // Cache miss counter
};

cache_t* cache_create(int capacity) {
    cache_t* cache = malloc(sizeof(cache_t));
    if (!cache) {
        fprintf(stderr, "Failed to allocate cache\n");
        exit(1);
    }
    
    cache->entries = malloc(capacity * sizeof(cache_entry));
    if (!cache->entries) {
        fprintf(stderr, "Failed to allocate cache entries\n");
        free(cache);
        exit(1);
    }
    
    cache->capacity = capacity;
    cache->size = 0;
    cache->insert_pos = 0;
    cache->hits = 0;
    cache->misses = 0;
    
    for (int i = 0; i < capacity; i++) {
        cache->entries[i].is_valid = 0;
    }
    
    return cache;
}

void cache_destroy(cache_t* cache) {
    if (cache) {
        free(cache->entries);
        free(cache);
    }
}

int cache_lookup(cache_t* cache, int key, int* value) {
    for (int i = 0; i < cache->capacity; i++) {
        if (cache->entries[i].is_valid && cache->entries[i].key == key) {
            *value = cache->entries[i].value;
            cache->hits++;
            return 1;  // Cache hit
        }
    }
    
    cache->misses++;
    return 0;  // Cache miss
}

void cache_insert(cache_t* cache, int key, int value) {
    // FIFO policy: replace oldest entry
    int pos = cache->insert_pos;
    
    cache->entries[pos].key = key;
    cache->entries[pos].value = value;
    cache->entries[pos].is_valid = 1;
    
    // Update insertion position for next time
    cache->insert_pos = (cache->insert_pos + 1) % cache->capacity;
    
    // Increase size if we haven't filled the cache yet
    if (cache->size < cache->capacity) {
        cache->size++;
    }
}

void cache_stats(cache_t* cache) {
    printf("Cache stats: %d hits, %d misses\n", cache->hits, cache->misses);
}