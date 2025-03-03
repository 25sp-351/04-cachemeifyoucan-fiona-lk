// Fiona Keaney
// fionakeaney@csu.fullerton.edu

#include <stdio.h>
#include <stdlib.h>

#include "cache.h"

#define CACHE_SIZE 100

typedef struct {
    int length;
    int value;
} piece_length_value;

cache_t* cache = NULL;

typedef struct {
    int value;
    int first_cut_index;
} solution;

// recursive rod cutting
int cut_rod(int rod_length, piece_length_value pieces[], int num_pieces,
            solution* solutions) {
    // base case
    if (rod_length == 0)
        return 0;

    // check
    int cached_value;
    if (cache_lookup(cache, rod_length, &cached_value))
        return cached_value;

    int max_profit     = 0;
    int best_cut_index = -1;

    for (int i = 0; i < num_pieces; i++) {
        if (pieces[i].length <= rod_length) {
            int current_profit =
                pieces[i].value + cut_rod(rod_length - pieces[i].length, pieces,
                                          num_pieces, solutions);
            if (current_profit > max_profit) {
                max_profit     = current_profit;
                best_cut_index = i;
            }
        }
    }

    solutions[rod_length].value           = max_profit;
    solutions[rod_length].first_cut_index = best_cut_index;

    // store result
    cache_insert(cache, rod_length, max_profit);

    return max_profit;
}

void get_cuts(int rod_length, piece_length_value pieces[], solution* solutions,
              int* cut_lengths, int* cut_count) {
    *cut_count           = 0;

    int remaining_length = rod_length;
    while (remaining_length > 0) {
        int idx = solutions[remaining_length].first_cut_index;
        if (idx == -1)
            break;

        cut_lengths[(*cut_count)++] = pieces[idx].length;
        remaining_length -= pieces[idx].length;
    }
}

void print_results(int cut_lengths[], int cut_count,
                   piece_length_value pieces[], int num_pieces,
                   int rod_length) {
    int total_value       = 0;
    int length_used       = 0;

    int length_counts[50] = {0};

    for (int i = 0; i < cut_count; i++) {
        int length = cut_lengths[i];
        length_counts[length]++;
        length_used += length;
    }

    // print results
    for (int i = 0; i < num_pieces; i++) {
        int length = pieces[i].length;
        int count  = length_counts[length];

        if (count > 0) {
            int value = count * pieces[i].value;
            total_value += value;
            printf("%d @ %d = %d\n", count, length, value);
        }
    }

    int remainder = rod_length - length_used;
    printf("Remainder: %d\n", remainder);
    printf("Value: %d\n", total_value);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <prices_file>\n", argv[0]);
        return 1;
    }

    // create cache
    cache = cache_create(CACHE_SIZE);
    if (!cache) {
        fprintf(stderr, "Failed to create cache\n");
        return 1;
    }

    // open prices file
    FILE* prices_file = fopen(argv[1], "r");
    if (!prices_file) {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        cache_destroy(cache);
        return 1;
    }

    // read
    piece_length_value pieces[50] = {0};
    int num_pieces                = 0;

    while (fscanf(prices_file, "%d, %d", &pieces[num_pieces].length,
                  &pieces[num_pieces].value) == 2) {
        num_pieces++;
        if (num_pieces >= 50) {
            fprintf(stderr, "Warning: Maximum number of pieces reached\n");
            break;
        }
    }

    fclose(prices_file);

    int rod_length;
    while (scanf("%d", &rod_length) == 1) {
        printf("Processing rod of length %d:\n", rod_length);

        solution* solutions = calloc(rod_length + 1, sizeof(solution));
        for (int i = 0; i <= rod_length; i++)
            solutions[i].first_cut_index = -1;

        cut_rod(rod_length, pieces, num_pieces, solutions);

        int* cut_lengths = calloc(rod_length, sizeof(int));
        int cut_count    = 0;
        get_cuts(rod_length, pieces, solutions, cut_lengths, &cut_count);

        // print results
        print_results(cut_lengths, cut_count, pieces, num_pieces, rod_length);
        printf("\n");

        free(solutions);
        free(cut_lengths);
    }

    cache_stats(cache);

    cache_destroy(cache);

    return 0;
}