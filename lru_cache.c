// Fiona Keaney
// fionakeaney@csu.fullerton.edu


#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

struct cache_struct {
    cache_entry* entries;
    int* usage_times; 
    int capacity;
    int size;
    int current_time;
    int hits;
    int misses;
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
    
    cache->usage_times = malloc(capacity * sizeof(int));
    if (!cache->usage_times) {
        fprintf(stderr, "Failed to allocate usage tracking\n");
        free(cache->entries);
        free(cache);
        exit(1);
    }
    
    cache->capacity = capacity;
    cache->size = 0;
    cache->current_time = 0;
    cache->hits = 0;
    cache->misses = 0;
    
    for (int i = 0; i < capacity; i++) {
        cache->entries[i].is_valid = 0;
        cache->usage_times[i] = 0;
    }
    
    return cache;
}

void cache_destroy(cache_t* cache) {
    if (cache) {
        free(cache->entries);
        free(cache->usage_times);
        free(cache);
    }
}

int cache_lookup(cache_t* cache, int key, int* value) {
    for (int i = 0; i < cache->capacity; i++) {
        if (cache->entries[i].is_valid && cache->entries[i].key == key) {
            *value = cache->entries[i].value;
            
            cache->usage_times[i] = ++cache->current_time;
            cache->hits++;
            
            return 1;
        }
    }
    
    cache->misses++;
    return 0;
}

void cache_insert(cache_t* cache, int key, int value) {
    int pos = -1;
    
    for (int i = 0; i < cache->capacity; i++) {
        if (cache->entries[i].is_valid && cache->entries[i].key == key) {
            pos = i;
            break;
        }
    }
    
    if (pos == -1) {
        if (cache->size < cache->capacity) {
            pos = cache->size;
            cache->size++;
        } else {
            int lru_time = cache->usage_times[0];
            pos = 0;
            
            for (int i = 1; i < cache->capacity; i++) {
                if (cache->usage_times[i] < lru_time) {
                    lru_time = cache->usage_times[i];
                    pos = i;
                }
            }
        }
    }
    
    cache->entries[pos].key = key;
    cache->entries[pos].value = value;
    cache->entries[pos].is_valid = 1;
    cache->usage_times[pos] = ++cache->current_time;
}

void cache_stats(cache_t* cache) {
    printf("Cache stats: %d hits, %d misses\n", cache->hits, cache->misses);
}