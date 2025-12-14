/* main.c */

#include <stdio.h>
#include <stdlib.h>
#include "axpy.h"

static int check_result(int n, const double *z, const double *expected)
{
    for (int i = 0; i < n; ++i) {
        double diff = z[i] - expected[i];
        if (diff < 0.0) diff = -diff;
        if (diff > 1e-12) {
            fprintf(stderr,
                    "Mismatch at index %d: z[i]=%.15g, expected=%.15g\n",
                    i, z[i], expected[i]);
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv)
{
    int n = 10;            /* default length for tests */
    double alpha = 2.0;    /* test scalar */

    if (argc > 1) {
        n = atoi(argv[1]);
        if (n <= 0) {
            fprintf(stderr, "n must be positive\n");
            return 1;
        }
    }

    printf("Testing axpy with n = %d, alpha = %.2f\n", n, alpha);

    double *x = malloc((size_t)n * sizeof(double));
    double *y = malloc((size_t)n * sizeof(double));
    double *z = malloc((size_t)n * sizeof(double));
    double *expected = malloc((size_t)n * sizeof(double));

    if (!x || !y || !z || !expected) {
        fprintf(stderr, "Memory allocation failed\n");
        free(x); free(y); free(z); free(expected);
        return 1;
    }

    /* --- Test 1: separate x, y, z --- */
    for (int i = 0; i < n; ++i) {
        x[i] = (double)i;
        y[i] = 1.0;
        z[i] = 0.0;
        expected[i] = alpha * x[i] + y[i];
    }

    axpy(n, alpha, x, y, z);

    if (!check_result(n, z, expected)) {
        fprintf(stderr, "Test 1 (separate z) FAILED\n");
        free(x); free(y); free(z); free(expected);
        return 1;
    }
    printf("Test 1 (separate z) passed.\n");

    /* --- Test 2: in-place on x (z == x) --- */
    for (int i = 0; i < n; ++i) {
        x[i] = (double)i;
        y[i] = 1.0;
        expected[i] = alpha * x[i] + y[i];  /* store expected BEFORE axpy */
    }

    axpy(n, alpha, x, y, x);  /* x will hold the result */

    if (!check_result(n, x, expected)) {
        fprintf(stderr, "Test 2 (in-place on x) FAILED\n");
        free(x); free(y); free(z); free(expected);
        return 1;
    }
    printf("Test 2 (in-place on x) passed.\n");

    free(x);
    free(y);
    free(z);
    free(expected);

    printf("All tests passed.\n");
    return 0;
}
