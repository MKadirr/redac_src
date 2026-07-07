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
    printf("binary");
}

// turn an array into suitable data structure to optimize find
void* init_find(int* arr, size_t len) {
    struct Data* data = calloc(sizeof(struct Data), 1);
    data->data = malloc(sizeof(int) * len);

    memcpy(data->data, arr, sizeof(int) * len);



    data->len = len;

    return data;
}


// use the previously defined data to search if the given value is present
int find(void* data, int value) {
    // printf("\nsearch: %d\n", value);
    struct Data* arr = (struct Data*)data;

    ssize_t a = 0;
    ssize_t b = arr->len - 1;
    ssize_t m;

    while (a <= b) {
        m = (a + b) / 2;

        // printf("a=%zu, m=%zu, b=%zu, value=%d\n", a, m, b, arr->data[m]);

        if (arr->data[m] == value) return arr->data[m];

        if (arr->data[m] > value) {
            b = m - 1;
        }
        else {
            a = m + 1;
        }
    }

    // printf("a=%zu, b=%zu, arr[a]=%d, arr[b]=%d\n", a, b, arr->data[a], arr->data[b]);

    return 0;
}

// free the query data structure
void free_find(void* data) {
    struct Data* arr = (struct Data*)data;

    free(arr->data);
    free(arr);
}