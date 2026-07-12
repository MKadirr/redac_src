#pragma GCC target("avx2")

#include "find.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <immintrin.h>

#define B 16
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
    size_t nblock;
    int* data;
    size_t H;
};

typedef __m256i reg;

int cmp_simd(reg x_vec, int* y_ptr) {
    reg y_vec = _mm256_load_si256((reg*) y_ptr);
    reg mask = _mm256_cmpgt_epi32(x_vec, y_vec);
    return _mm256_movemask_ps((__m256) mask);
}

unsigned rank(reg x, int* y) {
    reg a = _mm256_load_si256((reg*) y);
    reg b = _mm256_load_si256((reg*) (y + 8));

    reg ca = _mm256_cmpgt_epi32(a, x);
    reg cb = _mm256_cmpgt_epi32(b, x);

    reg c = _mm256_packs_epi32(ca, cb);
    int mask = _mm256_movemask_epi8(c);

    // we need to divide the result by two because we call movemask_epi8 on 16-bit masks:
    return __tzcnt_u32(mask) >> 1;
}

void permute(int *node) {
    const reg perm = _mm256_setr_epi32(4, 5, 6, 7, 0, 1, 2, 3);
    reg* middle = (reg*) (node + 4);
    reg x = _mm256_loadu_si256(middle);
    x = _mm256_permutevar8x32_epi32(x, perm);
    _mm256_storeu_si256(middle, x);
}

void pres_find() {
    printf("stree_plus");
}

size_t go(size_t k, size_t i) {
    return k * (B + 1) + i + 1;
}

int height(int n) {
    // grow the tree until its size exceeds n elements
    int s = 0, // total size so far
        l = B, // size of the next layer
        h = 0; // height so far
    while (s + l - B < n) {
        s += l;
        l *= (B + 1);
        h++;
    }
    return h;
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

    data->data = alloc_aligned(sizeof(int) * nblock * B);

    data->len = len;
    data->nblock = nblock;
    data->H = height(len);

    rec_build((int (*)[B])data->data, arr, 0, len, nblock, 0);

    // printf("nblock=%zu, H=%d\n", nblock, height(len));

    // Étape manquante : permuter chaque nœud pour que rank()/translate() soient cohérents
    int (*btree)[B] = (int (*)[B])data->data;
    for (size_t k = 0; k < nblock; k++) {
        permute(btree[k]);
    }

    return data;
}

const int translate[17] = {
    0, 1, 2, 3,
    8, 9, 10, 11,
    4, 5, 6, 7,
    12, 13, 14, 15,
    0
};

void update(int* res, int* node, unsigned i) {
    int val = node[translate[i]];
    *res = (i < B ? val : *res);
}

// use the previously defined data to search if the given value is present
int find(void* data, int value) {
    // printf("\nsearch: %d\n", value);
    struct Data* arr = (struct Data*)data;

    int (*btree)[B] = (int (*)[B])arr->data;

    size_t nblocks = arr->nblock;
    size_t H = arr->H;

    size_t k = 0;
    // size_t i = 0;

    int res = -1;

    reg x = _mm256_set1_epi32(value - 1);

    for (int h = 0; h < H; h++) {
        unsigned i = rank(x, btree[k]);
        // printf("  [h=%d] k=%zu, i=%u, node[translate[i]]=%d, res_avant=%d\n",
        //        h, k, i, (i < B ? btree[k][translate[i]] : -999), res);
        update(&res, btree[k], i);
        // printf("  [h=%d] res_apres=%d\n", h, res);
        k = go(k, i);
    }
    // the last branch:
    if (k < nblocks) {
        unsigned i = rank(x, btree[k]);
        // printf("  [final] k=%zu, i=%u, node[translate[i]]=%d, res_avant=%d\n",
        //        k, i, (i < B ? btree[k][translate[i]] : -999), res);
        update(&res, btree[k], i);
        // printf("  [final] res_apres=%d\n", res);
    }

    // if (res != value) {
    //     fprintf(stderr, "MISMATCH: recherché=%d, trouvé=%d\n", value, res);
    // }

    return res;
}

// free the query data structure
void free_find(void* data) {
    struct Data* arr = (struct Data*)data;

    free(arr->data);
    free(arr);
}