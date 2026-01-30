#include "solvers.h"

#include <stdlib.h>
#include <math.h>

int w_jacobi(const COOMatrix *A, const double *b, double omega, double *x, double *res) {
    if (!A || !b || !x || !res) return -1;
    if (A->n_rows <= 0 || A->n_cols <= 0) return -2;
    if (A->n_rows != A->n_cols) return -3;
    if (!(omega > 0.0 && omega <= 1.0)) return -4;

    const int n = A->n_rows;
    const int maxiter = 5000;
    const double tol = 1e-6;

    double *diag = (double*)malloc((size_t)n * sizeof(double));
    double *Ax   = (double*)malloc((size_t)n * sizeof(double));
    double *r    = (double*)malloc((size_t)n * sizeof(double));
    if (!diag || !Ax || !r) {
        free(diag); free(Ax); free(r);
        return -5;
    }

    coo_extract_diag(A, diag);

    /* Jacobi requires invertible diagonal */
    for (int i = 0; i < n; ++i) {
        if (diag[i] == 0.0) {
            free(diag); free(Ax); free(r);
            return -6;
        }
    }

    int it;
    for (it = 0; it < maxiter; ++it) {
        /* Ax = A*x */
        spmv(A, x, Ax);

        /* r = b - Ax */
        for (int i = 0; i < n; ++i) r[i] = b[i] - Ax[i];

        /* residual norm */
        *res = norm2(r, n);

        /* Bullet-proof: detect divergence/overflow */
        if (!isfinite(*res)) {
            free(diag); free(Ax); free(r);
            return -7;
        }

        if (*res < tol) break;

        /* x = x + omega * D^{-1} r */
        for (int i = 0; i < n; ++i) {
            x[i] += omega * (r[i] / diag[i]);
        }
    }

    /* If maxed out, recompute a final finite residual if possible */
    if (it == maxiter) {
        spmv(A, x, Ax);
        for (int i = 0; i < n; ++i) r[i] = b[i] - Ax[i];
        *res = norm2(r, n);
        if (!isfinite(*res)) {
            free(diag); free(Ax); free(r);
            return -7;
        }
    }

    free(diag);
    free(Ax);
    free(r);

    return it; /* 0..maxiter */
}