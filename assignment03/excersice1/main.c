#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#include <cblas.h>

#include "matrix.h"
#include "dense_solvers.h"

static double vec_norm2(int n, const double *v) {
    double s = 0.0;
    for (int i = 0; i < n; ++i) s += v[i] * v[i];
    return sqrt(s);
}

static double vec_norminf(int n, const double *v) {
    double m = 0.0;
    for (int i = 0; i < n; ++i) {
        double a = fabs(v[i]);
        if (a > m) m = a;
    }
    return m;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s dense_mat.txt\n", argv[0]);
        return 1;
    }

    Matrix A;
    if (parse_matrix(argv[1], &A)) {
        fprintf(stderr, "Matrix file '%s' is invalid or does not exist.\n", argv[1]);
        return 2;
    }

    assert(A.ncols == A.nrows);
    int n = (int)A.nrows;

    double *x_exact = (double*)malloc((size_t)n * sizeof(double));
    double *b       = (double*)malloc((size_t)n * sizeof(double));
    double *x_tilde = (double*)malloc((size_t)n * sizeof(double));
    if (!x_exact || !b || !x_tilde) {
        fprintf(stderr, "Allocation failed.\n");
        free(x_exact); free(b); free(x_tilde);
        free_matrix(&A);
        return 3;
    }

    for (int i = 0; i < n; ++i) {
        x_exact[i] = 1.0;
        x_tilde[i] = 0.0;
    }

    cblas_dgemv(CblasColMajor, CblasNoTrans, n, n,
                1.0, A.data, n, x_exact, 1,
                0.0, b, 1);

    int maxiter = 200;
    int rc = mp_iter_refinement(n, A.data, b, x_tilde, maxiter);
    if (rc != 0) {
        fprintf(stderr, "mp_iter_refinement failed with code %d\n", rc);
        free(x_exact); free(b); free(x_tilde);
        free_matrix(&A);
        return 4;
    }

    double *Ax = (double*)malloc((size_t)n * sizeof(double));
    double *r  = (double*)malloc((size_t)n * sizeof(double));
    if (!Ax || !r) {
        fprintf(stderr, "Allocation failed.\n");
        free(Ax); free(r);
        free(x_exact); free(b); free(x_tilde);
        free_matrix(&A);
        return 5;
    }

    cblas_dgemv(CblasColMajor, CblasNoTrans, n, n,
                1.0, A.data, n, x_tilde, 1,
                0.0, Ax, 1);
    for (int i = 0; i < n; ++i) r[i] = b[i] - Ax[i];

    double err_inf = 0.0;
    for (int i = 0; i < n; ++i) {
        double diff = fabs(x_exact[i] - x_tilde[i]);
        if (diff > err_inf) err_inf = diff;
    }
    double x_inf = vec_norminf(n, x_exact);
    double rel_sheet = (x_inf == 0.0) ? 0.0 : (err_inf / x_inf);

    double *e = (double*)malloc((size_t)n * sizeof(double));
    if (!e) {
        fprintf(stderr, "Allocation failed.\n");
        free(Ax); free(r);
        free(x_exact); free(b); free(x_tilde);
        free_matrix(&A);
        return 6;
    }
    for (int i = 0; i < n; ++i) e[i] = x_tilde[i] - x_exact[i];
    double rel_sol_err_2 = vec_norm2(n, e) / vec_norm2(n, x_exact);

    printf("mp_iter_refinement iterations: %d\n", mp_last_iterations);
    printf("n = %d\n", n);
    printf("relative solution error ||x_tilde - x||_2 / ||x||_2 = %.16e\n", rel_sol_err_2);
    printf("residual norm ||b - A x_tilde||_2 = %.16e\n", vec_norm2(n, r));
    printf("relative backward error (sheet) ||x - x_tilde||_inf / ||x||_inf = %.16e\n", rel_sheet);

    free(e);
    free(Ax);
    free(r);
    free(x_exact);
    free(b);
    free(x_tilde);
    free_matrix(&A);
    return 0;
}