#include <stdio.h>
#include <stdlib.h>

#include "coo.h"
#include "solvers.h"

static void run_case(const COOMatrix *A, const double *b, double omega) {
    int n = A->n_rows;

    double *x = (double*)malloc((size_t)n * sizeof(double));
    if (!x) {
        fprintf(stderr, "malloc failed\n");
        return;
    }

    zeros(x, n); /* x0 = 0 */
    double res = 0.0;

    int it = w_jacobi(A, b, omega, x, &res);

    if (it >= 0 && it < 5000) {
        printf("omega = %.10g | iterations = %d | final ||r||_2 = %.16e\n", omega, it, res);
    } else if (it == 5000) {
        printf("omega = %.10g | iterations = %d (max) | final ||r||_2 = %.16e (not converged)\n",
               omega, it, res);
    } else {
        printf("omega = %.10g | failed (code %d) | final ||r||_2 = %.16e\n", omega, it, res);
    }

    free(x);
}

int main(int argc, char **argv) {
    const char *path = "test_mat.txt";
    if (argc >= 2) path = argv[1];

    COOMatrix A = {0};
    if (read_coo(path, &A) != 0) {
        fprintf(stderr, "read_coo failed for '%s'\n", path);
        return 1;
    }

    if (A.n_rows != A.n_cols) {
        fprintf(stderr, "Matrix must be square for Jacobi (got %d x %d)\n", A.n_rows, A.n_cols);
        free_coo(&A);
        return 2;
    }

    int n = A.n_rows;
    double *b = (double*)malloc((size_t)n * sizeof(double));
    if (!b) {
        fprintf(stderr, "malloc failed\n");
        free_coo(&A);
        return 3;
    }
    ones(b, n); /* b = 1 */

    run_case(&A, b, 1.0/3.0);
    run_case(&A, b, 2.0/3.0);
    run_case(&A, b, 1.0);

    free(b);
    free_coo(&A);
    return 0;
}