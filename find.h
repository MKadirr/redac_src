#pragma once

#include <stddef.h>

// this function is used for display only it is only made to automatically present the layout
void pres_find();

// turn an array into suitable data structure to optimize find
void* init_find(int* arr, size_t len);

// use the previously defined data to search if the given value is present
int find(void* data, int value);

// free the query data structure
void free_find(void* data);
