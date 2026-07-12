#include "find.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CACHE_LINE_SIZE 64

void* alloc_aligned(size_t size) {
    // La taille doit être un multiple de l'alignement avec aligned_alloc
    size_t aligned_size = (size + CACHE_LINE_SIZE - 1) & ~(CACHE_LINE_SIZE - 1);
    void* ptr = aligned_alloc(CACHE_LINE_SIZE, aligned_size);
    if (!ptr) {
        // gestion d'erreur
        return NULL;
    }
    return ptr;
}

struct Data {
    size_t len;
    int* data;
};

void pres_find() {
    printf("eytzinger_prefetch");
}

int rec_build(int* dst, int* src, size_t n, size_t i, size_t k) {
    if (k <= n) {
        i = rec_build(dst, src, n, i, 2 * k);
        dst[k] = src[i++];
        i = rec_build(dst, src, n, i, 2 * k + 1);
    }
    return i;
}

// turn an array into suitable data structure to optimize find
void* init_find(int* arr, size_t len) {
    struct Data* data = calloc(sizeof(struct Data), 1);
    data->data = alloc_aligned(sizeof(int) * (len + 1));

    data->len = len;

    rec_build(data->data, arr, len, 0, 1);

    // for (int i = 0; i < len + 1; i++) {
    //     printf(", %d", data->data[i]);
    // }
    // printf("\n");

    return data;
}

#define L 8

// use the previously defined data to search if the given value is present
int find(void* data, int value) {
    struct Data* data_struct = (struct Data*)data;

    size_t i = 1;
    size_t len = data_struct->len;
    int* arr = data_struct->data;

    size_t tmp = 1;
    while (tmp < L && (1 << tmp) < len) {
        __builtin_prefetch(arr + (1 << tmp));
        tmp++;
    }

    while (i <= len) {
        int val = arr[i];

        if (val == value) {
            return val;
        }

        // __builtin_prefetch(arr->data + i*2);

        if (value < val) {
            // printf("smaller: value = %d, arr[m]=%d\n", value, arr->data[i]);
            i = i * 2;
        }
        else {
            // printf("bigger: value = %d, arr[m]=%d\n", value, arr->data[i]);
            i = i * 2 + 1;
        }
    }

    // printf("i=%zu, len=%zu, arr[i]=%d\n", i, arr->len, arr->data[i]);

    return -1;
}

// free the query data structure
void free_find(void* data) {
    struct Data* arr = (struct Data*)data;

    free(arr->data);
    free(arr);
}