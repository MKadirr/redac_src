#include "find.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define B 16

struct Data {
    size_t len;
    size_t nblock;
    int* data;
};

void pres_find() {
    printf("stree");
}

size_t go(size_t k, size_t i) {
    return k * (B + 1) + i + 1;
}

size_t rec_build(int (*dst)[B], int* src, size_t t, size_t n, size_t nblock,  size_t k) {
    // printf("n=%d\n", n);
    // static int t = 0;
    if (k < nblock) {
        for (int i = 0; i < B; i++){
            // printf("k=%zu\n", k);
            t = rec_build(dst, src, t, n, nblock, go(k, i));
            dst[k][i] = (t < n ? src[t++] : INT_MAX);
        }
        t = rec_build(dst, src, t, n, nblock, go(k, B));
    }

    return t;
}

// turn an array into suitable data structure to optimize find
void* init_find(int* arr, size_t len) {
    struct Data* data = calloc(sizeof(struct Data), 1);

    size_t nblock = (len + B - 1) / B;

    data->data = malloc(sizeof(int) * nblock * B);

    data->len = len;
    data->nblock = nblock;

    rec_build((int (*)[B])data->data, arr, 0, len, nblock, 0);

    // int (*tmp)[B] = (int (*)[B])data->data;
    // for (int k = 0; k < nblock; k++) {
    //     for (int i = 0; i < B; i++) {
    //         printf(", %d", tmp[k][i]);
    //     }
    //     printf("\n");
    // }

    return data;
}


// use the previously defined data to search if the given value is present
int find(void* data, int value) {
    // printf("\nsearch: %d\n", value);
    struct Data* arr = (struct Data*)data;

    int (*btree)[B] = (int (*)[B])arr->data;

    size_t k = 0;
    size_t i = 0;

    while (k < arr->nblock) {

        // printf("block %zu: ", k);
        // for (int i = 0; i < B; i++) {
        //     printf("%d ,", btree[k][i]);
        // }
        // printf("\n");

        size_t i = 0;
        for (size_t ii = 0; ii < B; ii++) {
            i += btree[k][ii] < value;
        }

        if (i < B && btree[k][i] == value) {
            // printf("found: %d\n", btree[k][i]);
            return btree[k][i];
        }

        // printf("k=%zu, i=%zu, arr[k][i]=%d, arr[k][i+1]=%d\n", k, i, btree[k][i], btree[k][1 + i]);
        k = go(k, i);
    }

    return -1;
}

// free the query data structure
void free_find(void* data) {
    struct Data* arr = (struct Data*)data;

    free(arr->data);
    free(arr);
}