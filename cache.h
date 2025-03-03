// Fiona Keaney
// fionakeaney@csu.fullerton.edu


#ifndef CACHE_H
#define CACHE_H

// Cache entry structure
typedef struct {
    int key;           // Rod length
    int value;         // Optimal value for this rod length
    int is_valid;      // Flag to indicate if entry is valid
} cache_entry;

// Cache interface
typedef struct cache_struct cache_t;

// Cache operations
cache_t* cache_create(int capacity);
void cache_destroy(cache_t* cache);
int cache_lookup(cache_t* cache, int key, int* value);
void cache_insert(cache_t* cache, int key, int value);
void cache_stats(cache_t* cache);

#endif /* CACHE_H */