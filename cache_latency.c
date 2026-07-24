#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STRIDE 64  // taille de ligne de cache

void bench_latency(size_t size_bytes) {
    size_t n = size_bytes / sizeof(void*);
    void** arr = malloc(size_bytes);

    // construire une permutation aléatoire de type "pointeur suivant"
    size_t* indices = malloc(n * sizeof(size_t));
    for (size_t i = 0; i < n; i++) indices[i] = i;
    for (size_t i = n - 1; i > 0; i--) {  // shuffle Fisher-Yates
        size_t j = rand() % (i + 1);
        size_t tmp = indices[i]; indices[i] = indices[j]; indices[j] = tmp;
    }
    for (size_t i = 0; i < n; i++) {
        arr[indices[i]] = &arr[indices[(i + 1) % n]];
    }

    // warm-up
    void** p = arr[0];
    for (size_t i = 0; i < n; i++) p = *p;

    // mesure
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    size_t iterations = 10000000;
    for (size_t i = 0; i < iterations; i++) {
        p = *p;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double ns = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
    printf("Taille: %8zu KB -> latence: %.2f ns/accès\n",
           size_bytes / 1024, ns / iterations);

    free(arr);
    free(indices);
}

int main(void) {
    size_t sizes[] = {
        4 * 1024,        // 4 KB, doit tenir en L1
        32 * 1024,       // 32 KB, limite L1 typique
        256 * 1024,      // 256 KB, limite L2 typique
        16 * 1024 * 1024, // 8 MB, limite L3 typique
        128 * 1024 * 1024 // 128 MB, force DRAM
    };
    for (int i = 0; i < 5; i++) {
        bench_latency(sizes[i]);
    }
    return 0;
}