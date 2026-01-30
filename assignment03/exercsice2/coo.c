#include "coo.h"

#include <stdlib.h>
#include <math.h>

int read_coo(const char *filename, COOMatrix *A) {
    if (!filename || !A) {
        return -1;
    }

    FILE *f = fopen(filename, "r");
    if (!f) {
        return -2;
    }

    int m = 0, n = 0, nnz = 0;
    if (fscanf(f, "%d %d %d", &m, &n, &nnz) != 3) {
        fclose(f);
        return -3;
    }

    A->n_rows = m;
    A->n_cols = n;
    A->nnz = nnz;

    A->row_indices = (int *)malloc((size_t)nnz * sizeof(int));
    A->col_indices = (int *)malloc((size_t)nnz * sizeof(int));
    A->values = (double *)malloc((size_t)nnz * sizeof(double));
    if (!A->row_indices || !A->col_indices || !A->values) {
        fclose(f);
        free(A->row_indices);
        free(A->col_indices);
        free(A->values);
        return -4;
    }

    for (int k = 0; k < nnz; ++k) {
        int r = 0, c = 0;
        double v = 0.0;
        if (fscanf(f, "%d %d %lf", &r, &c, &v) != 3) {
            fclose(f);
            free(A->row_indices);
            free(A->col_indices);
            free(A->values);
            return -5;
        }
        A->row_indices[k] = r;
        A->col_indices[k] = c;
        A->values[k] = v;
    }

    fclose(f);
    return 0;
}

void free_coo(COOMatrix *A) {
    if (!A) return;
    free(A->row_indices);
    free(A->col_indices);
    free(A->values);
    A->row_indices = NULL;
    A->col_indices = NULL;
    A->values = NULL;
    A->nnz = 0;
    A->n_rows = 0;
    A->n_cols = 0;
}

double norm2(const double *x, int n) {
    if (!x || n <= 0) return 0.0;
    double s = 0.0;
    for (int i = 0; i < n; ++i) {
        s += x[i] * x[i];
    }
    return sqrt(s);
}

void zeros(double *x, int n) {
    if (!x || n <= 0) return;
    for (int i = 0; i < n; ++i) x[i] = 0.0;
}

void ones(double *x, int n) {
    if (!x || n <= 0) return;
    for (int i = 0; i < n; ++i) x[i] = 1.0;
}

void spmv(const COOMatrix *A, const double *x, double *y) {
    if (!A || !x || !y) return;
    if (A->n_rows <= 0 || A->n_cols <= 0 || A->nnz < 0) return;

    /* y = 0 */
    zeros(y, A->n_rows);

    /* y[row_k] += val_k * x[col_k] */
    for (int k = 0; k < A->nnz; ++k) {
        int r = A->row_indices[k];
        int c = A->col_indices[k];

        /* basic bounds safety (in case input file is malformed) */
        if (r < 0 || r >= A->n_rows) continue;
        if (c < 0 || c >= A->n_cols) continue;

        y[r] += A->values[k] * x[c];
    }
}

void coo_extract_diag(const COOMatrix *A, double *diag) {
    if (!A || !diag) return;
    if (A->n_rows <= 0 || A->n_cols <= 0 || A->nnz < 0) return;

    /* initialize diag to 0 (if diagonal entry absent, keep 0) */
    zeros(diag, A->n_rows);

    /* diag[i] += A(i,i) if present; if duplicates exist, sum them */
    for (int k = 0; k < A->nnz; ++k) {
        int r = A->row_indices[k];
        int c = A->col_indices[k];
        if (r < 0 || r >= A->n_rows) continue;
        if (c < 0 || c >= A->n_cols) continue;

        if (r == c) {
            diag[r] += A->values[k];
        }
    }
}