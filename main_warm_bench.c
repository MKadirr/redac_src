#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define _POSIX_C_SOURCE 199309L

#include <time.h>
#include <sys/time.h>
#include <math.h>

#include "find.h"

// number of time the query is called
#define NB_QUERY 703L

int cmp(const void* a, const void* b) {
    return (*(const int*)a) - (*(const int *)b);
}

int* random_data(size_t len) {
    int* res = calloc(sizeof(int), len);

    for (size_t i = 0; i < len; i++) {
		res[i] = i;
        // res[i] = random() % 0xffffffff;
    }

	// for (int i = 0; i < len; i++) rse[i] = i * 1000 + 7;

	// qsort(res, len, sizeof(int), cmp);

    return res;
}

struct Stats {
    double mean;
    double var;
    double std;
};

struct Stats proba(size_t* data, size_t len) {
    double mean = 0;

    for (size_t i = 0; i < len; i++) {
        mean += data[i];
    }

    mean /= len;

    double var = 0;
    double tmp;

    for (size_t i = 0; i < len; i++) {
        tmp = (data[i] - mean);
        var += tmp * tmp;
    }

    var /= len;

    struct Stats stats = {
        .mean = mean,
        .var = var,
        .std = sqrt(var)
    };

    return stats;
}

size_t arr_sizes[] = {
	536870912,  // aaaaa
};

int main(int argc, char**argv) {
    size_t nb_size = sizeof(arr_sizes) / sizeof(size_t);

	if (argc > 1) {
		printf("X");
		for (size_t i = 0; i < NB_QUERY; i++) {
			printf(",%zu", i);
		}
		printf("\n");
		return 0;
	}

    srandom(time(NULL));

	int* arr = random_data(arr_sizes[nb_size - 1]);


    // pres_find();
	fflush(stdout);

    for (size_t i = 0; i < nb_size; i++) {
		printf("%d", random());
        size_t len = arr_sizes[i];

        void* query_struct = init_find(arr, len);

		struct timespec start;
        struct timespec end;

		size_t data[NB_QUERY];
        for (size_t j = 0; j < NB_QUERY; j++) {

            size_t value = random() % len; // TODO define a value
			// value = 2112;

			// printf("start\n");
			clock_gettime(CLOCK_REALTIME, &start);
            int found = find(query_struct, value);
			clock_gettime(CLOCK_REALTIME, &end);

			if (found != value) {
				printf("fuck c'est pas la bonne valeur: expected: %d, found: %d\n", value, found);
			}
			// printf("end\n");
            
			size_t m_start = start.tv_sec * 1000000000 + start.tv_nsec;
			size_t m_end = end.tv_sec * 1000000000 + end.tv_nsec;

            // micro_s[j] = m_end - m_start;
			printf(",%zu", m_end - m_start);
			// printf("%zu, %zu => %zu\n", m_start, m_end, m_end - m_start);
			// printf("%Lf\n", mean);
        }
		
	
        free_find(query_struct);



        // struct Stats stats = proba(micro_s, NB_QUERY);
		// printf(",%f", stats.mean);

		// printf(",%Lf", mean / (double)NB_QUERY);
		// fflush(stdout);

        // printf("%d, %f\n", len, (double)(m_end - m_start) / (double)NB_QUERY);

        // TODO value
    }

	free(arr);
	printf("\n");
}