/* main.c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "axpy.h"

/* Return wall-clock time in seconds */
double wtime(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

int main(int argc, char **argv)
{
    int n;
    const int repetitions = 50;
    const double alpha = 2.0;

    if (argc < 2) {
        /* Default to 1,000,000 if no parameter is given */
        n = 1000000;
    } else {
        n = atoi(argv[1]);
        if (n <= 0) {
            fprintf(stderr, "n must be positive\n");
            return 1;
        }
    }

    printf("Timing axpy with n = %d, alpha = %.2f, repetitions = %d\n",
           n, alpha, repetitions);

    double *x = malloc((size_t)n * sizeof(double));
    double *y = malloc((size_t)n * sizeof(double));
    double *z = malloc((size_t)n * sizeof(double));

    if (!x || !y || !z) {
        fprintf(stderr, "Memory allocation failed\n");
        free(x); free(y); free(z);
        return 1;
    }

    /* Initialize x and y with some values of our choice */
    for (int i = 0; i < n; ++i) {
        x[i] = 1.0;
        y[i] = 2.0;
        z[i] = 0.0;
    }

    /* Optional warm-up call (not measured) */
    axpy(n, alpha, x, y, z);

    double sum = 0.0;
    double min = 0.0;
    double max = 0.0;

    for (int k = 0; k < repetitions; ++k) {
        double t0 = wtime();
        axpy(n, alpha, x, y, z);
        double t1 = wtime();
        double dt = t1 - t0;

        if (k == 0 || dt < min) min = dt;
        if (k == 0 || dt > max) max = dt;
        sum += dt;

        printf("Run %2d: %g seconds\n", k + 1, dt);
    }

    double avg = sum / repetitions;

    printf("Summary for n = %d:\n", n);
    printf("  min time: %g s\n", min);
    printf("  max time: %g s\n", max);
    printf("  avg time: %g s (used as trustworthy measurement)\n", avg);

    free(x);
    free(y);
    free(z);

    return 0;
}
