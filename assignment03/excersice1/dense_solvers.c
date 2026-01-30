#include "dense_solvers.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <cblas.h>
#include <lapacke.h>

int mp_last_iterations = -1;

int mp_iter_refinement(int n, double *A, double *b, double *x, int maxiter) {
    mp_last_iterations = -1;

    if (n <= 0 || !A || !b || !x) return 1;
    if (maxiter <= 10) return 2;

    const double tau = 1e-13;

    float *AsLU = (float*)malloc((size_t)n*(size_t)n*sizeof(float));
    float *bs   = (float*)malloc((size_t)n*sizeof(float));
    float *ds   = (float*)malloc((size_t)n*sizeof(float));
    int   *ipiv = (int*)malloc((size_t)n*sizeof(int));

    double *Ax  = (double*)malloc((size_t)n*sizeof(double));
    double *r   = (double*)malloc((size_t)n*sizeof(double));

    if (!AsLU || !bs || !ds || !ipiv || !Ax || !r) {
        free(AsLU); free(bs); free(ds); free(ipiv); free(Ax); free(r);
        return 3;
    }

    for (int i = 0; i < n*n; i++) AsLU[i] = (float)A[i];
    for (int i = 0; i < n; i++)   bs[i]   = (float)b[i];

    int info = LAPACKE_sgetrf(LAPACK_COL_MAJOR, n, n, AsLU, n, ipiv);
    if (info != 0) {
        free(AsLU); free(bs); free(ds); free(ipiv); free(Ax); free(r);
        return 4;
    }

    memcpy(ds, bs, (size_t)n*sizeof(float));
    info = LAPACKE_sgetrs(LAPACK_COL_MAJOR, 'N', n, 1, AsLU, n, ipiv, ds, n);
    if (info != 0) {
        free(AsLU); free(bs); free(ds); free(ipiv); free(Ax); free(r);
        return 5;
    }
    for (int i = 0; i < n; i++) x[i] = (double)ds[i];

    int iter;
    for (iter = 0; iter < maxiter; iter++) {
        cblas_dgemv(CblasColMajor, CblasNoTrans, n, n,
                    1.0, A, n, x, 1,
                    0.0, Ax, 1);

        for (int i = 0; i < n; i++) r[i] = b[i] - Ax[i];

        double rnorm2 = 0.0;
        for (int i = 0; i < n; i++) rnorm2 += r[i]*r[i];
        double rnorm = sqrt(rnorm2);
        if (rnorm < tau) break;

        for (int i = 0; i < n; i++) ds[i] = (float)r[i];

        info = LAPACKE_sgetrs(LAPACK_COL_MAJOR, 'N', n, 1, AsLU, n, ipiv, ds, n);
        if (info != 0) {
            free(AsLU); free(bs); free(ds); free(ipiv); free(Ax); free(r);
            return 6;
        }

        for (int i = 0; i < n; i++) x[i] += (double)ds[i];
    }

    mp_last_iterations = iter;

    free(AsLU); free(bs); free(ds); free(ipiv); free(Ax); free(r);

    if (iter == maxiter) return 7;
    return 0;
}