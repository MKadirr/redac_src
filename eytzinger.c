#include "find.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct Data {
    size_t len;
    int* data;
};

void pres_find() {
    printf("eytzinger");
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
    data->data = malloc(sizeof(int) * (len + 1));

    data->len = len;

    rec_build(data->data, arr, len, 0, 1);

    // for (int i = 0; i < len + 1; i++) {
    //     printf(", %d", data->data[i]);
    // }
    // printf("\n");

    return data;
}


// use the previously defined data to search if the given value is present
int find(void* data, int value) {
    struct Data* arr = (struct Data*)data;

    size_t i = 1;

    while (i <= arr->len) {
        if (arr->data[i] == value) {
            return arr->data[i];
        }

        if (value < arr->data[i]) {
            // printf("smaller: value = %d, arr[m]=%d\n", value, arr->data[i]);
            i = i * 2;
        }
        else {
            // printf("bigger: value = %d, arr[m]=%d\n", value, arr->data[i]);
            i = i * 2 + 1;
        }
    }

    // printf("i=%zu, len=%zu, arr[i]=%d\n", i, arr->len, arr->data[i]);

    return 0;
}

// free the query data structure
void free_find(void* data) {
    struct Data* arr = (struct Data*)data;

    free(arr->data);
    free(arr);
}