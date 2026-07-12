#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define _POSIX_C_SOURCE 199309L

#include <time.h>
#include <sys/time.h>
#include <math.h>

#include "find.h"

// number of time the query is called
#define NB_QUERY_WARMUP 10000L
#define NB_QUERY 10000L

int cmp(const void* a, const void* b) {
    return (*(const int*)a) - (*(const int *)b);
}

int* random_data(size_t len) {
    int* res = calloc(sizeof(int), len);

    for (size_t i = 0; i < len; i++) {
		res[i] = i;
        // res[i] = random() % 0xffffffff;
    }

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
    // 16,
	// 17,
	// 18,
	// 19,
	// 21,
	// 22,
	// 24,
	// 25,
	// 27,
	// 29,
	// 32,
	// 34,
	// 36,
	// 39,
	// 42,
	// 45,
	// 48,
	// 51,
	// 55,
	// 59,
	// 64,
	// 68,
	// 73,
	// 78,
	// 84,
	// 90,
	// 97,
	// 103,
	// 111,
	// 119,
	// 128,
	// 137,
	// 147,
	// 157,
	// 168,
	// 181,
	// 194,
	// 207,
	// 222,
	// 238,
	// 256,
	// 274,
	// 294,
	// 315,
	// 337,
	// 362,
	// 388,
	// 415,
	// 445,
	// 477,
	// 512,
	// 548,
	// 588,
	// 630,
	// 675,
	// 724,
	// 776,
	// 831,
	// 891,
	// 955,
	// 1024,
	// 1097,
	// 1176,
	// 1260,
	// 1351,
	// 1448,
	// 1552,
	// 1663,
	// 1782,
	// 1910,
	// 2048,
	// 2194,
	// 2352,
	// 2521,
	// 2702,
	// 2896,
	// 3104,
	// 3326,
	// 3565,
	// 3821,
	// 4096,
	// 4389,
	// 4705,
	// 5042,
	// 5404,
	// 5792,
	// 6208,
	// 6653,
	// 7131,
	// 7643,
	// 8192,
	// 8779,
	// 9410,
	// 10085,
	// 10809,
	// 11585,
	// 12416,
	// 13307,
	// 14263,
	// 15286,
	// 16384,
	// 17559,
	// 18820,
	// 20171,
	// 21618,
	// 23170,
	// 24833,
	// 26615,
	// 28526,
	// 30573,
	// 32768,
	// 35119,
	// 37640,
	// 40342,
	// 43237,
	// 46340,
	// 49667,
	// 53231,
	// 57052,
	// 61147,
	// 65536, // L1
	// 70239,
	// 75281,
	// 80684,
	// 86475,
	// 92681,
	// 99334,
	// 106463,
	// 114104,
	// 122294,
	// 131072,
	// 140479,
	// 150562,
	// 161368,
	// 172950,
	// 185363,
	// 198668,
	// 212927,
	// 228209,
	// 244589,
	// 262144,
	// 280958,
	// 301124,
	// 322737,
	// 345901,
	// 370727,
	// 397336,
	// 425854,
	// 456419,
	// 489178,
	// 524288,
	// 561917,
	// 602248,
	// 645474,
	// 691802,
	// 741455,
	// 794672,
	// 851708,
	// 912838,
	// 978356,
	// 1048576, // L2
	// 1123835,
	// 1204497,
	// 1290948,
	// 1383604,
	// 1482910,
	// 1589344,
	// 1703416,
	// 1825676,
	// 1956712,
	// 2097152,
	// 2247671,
	// 2408995,
	// 2581896,
	// 2767208,
	// 2965820,
	// 3178688,
	// 3406833,
	// 3651353,
	// 3913424,
	// 4194304, // L3
	// 4495343,
	// 4817990,
	// 5163793,
	// 5534417,
	// 5931641,
	// 6357376,
	// 6813666,
	// 7302707,
	// 7826848,
	// 8388608,
	// 8990687,
	// 9635980,
	// 10327587,
	// 11068834,
	// 11863283,
	// 12714752,
	// 13627333,
	// 14605414,
	// 15653696,
	// 16777216,
	// 17981374,
	// 19271960,
	// 20655175,
	// 22137669,
	// 23726566,
	// 25429504,
	// 27254667,
	// 29210829,
	// 31307392,
	// 33554432,
	// 35962749,
	// 38543920,
	// 41310351,
	// 44275338,
	// 47453132,
	// 50859008,
	// 54509335,
	// 58421659,
	// 62614784,
	// 67108864,
	// 71925499,
	// 77087841,
	// 82620702,
	// 88550676,
	// 94906265,
	// 101718016,
	// 109018671,
	// 116843318,
	// 125229568,
	// 134217728,
	// 143850999,
	// 154175683,
	// 165241405,
	// 177101353,
	// 189812531,
	// 203436033,
	// 218037342,
	// 233686637,
	// 250459136,
	// 268435456,
	// 287701998,
	// 308351366,
	// 330482811,
	// 354202707,
	// 379625062,
	// 406872067,
	// 436074684,
	// 467373274,
	// 500918273,
	// 536870912,  // aaaaa
	// 575403996,
	616702733,
	660965623,
	708405415,
	// 759250124,
	// 813744135,
	// 872149369,
	// 934746549,
	// 1001836546
	// 1073741824,
	// 1150807992,
	// 1233405466,
	// 1321931247,
	// 1416810830,
	// 1518500249,
	// 1627488270,
	// 1744298739,
	// 1869493099,
	// 2003673092
};

int main(int argc, char**argv) {
    size_t nb_size = sizeof(arr_sizes) / sizeof(size_t);

	if (argc > 1) {
		printf("X");
		for (size_t i = 0; i < nb_size; i++) {
			printf(",%zu", arr_sizes[i]);
		}
		printf("\n");
		return 0;
	}

    srandom(time(NULL));

	int* arr = random_data(arr_sizes[nb_size - 1]);


    pres_find();
	fflush(stdout);

    for (size_t i = 0; i < nb_size; i++) {
        size_t len = arr_sizes[i];

        void* query_struct = init_find(arr, len);
        // free(arr);

        // size_t micro_s[NB_QUERY];

		long double mean = 0;

		struct timespec start;
        struct timespec end;

		for (size_t j = 0; j < NB_QUERY_WARMUP; j++) {

            size_t value = random() % len; // TODO define a value

            int found = find(query_struct, value);

			if (found != value) {
				printf("fuck c'est pas la bonne valeur: expected: %d, found: %d\n", value, found);
			}
        }

		
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
			mean += m_end - m_start;
			// printf("%zu, %zu => %zu\n", m_start, m_end, m_end - m_start);
			// printf("%Lf\n", mean);
        }
		
	
        free_find(query_struct);



        // struct Stats stats = proba(micro_s, NB_QUERY);
		// printf(",%f", stats.mean);

		printf(",%Lf", mean / (double)NB_QUERY);
		fflush(stdout);

        // printf("%d, %f\n", len, (double)(m_end - m_start) / (double)NB_QUERY);

        // TODO value
    }

	free(arr);
	printf("\n");
}