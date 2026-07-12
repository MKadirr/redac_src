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
    struct Data* arr = (struct Data*)data;

    ssize_t a = 0;
    ssize_t b = arr->len - 1;
    ssize_t m;

    while (a <= b) {

        m = (a + b) / 2;
        int val = arr->data[m];

        if (val == value) {
            return val;
        }

        if (val > value) {
            b = m - 1;
        }
        else {
            a = m + 1;
        }
    }

    return -1;
}

// free the query data structure
void free_find(void* data) {
    struct Data* arr = (struct Data*)data;

    free(arr->data);
    free(arr);
}