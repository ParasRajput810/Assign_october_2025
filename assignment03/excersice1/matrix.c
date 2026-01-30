#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

void free_matrix(Matrix *matrix) {
    free(matrix->data);
}

int parse_matrix(const char *path, Matrix *matrix) {
    if (!path || !matrix) return -1;

    FILE *file = fopen(path, "r");
    if (!file) return -2;

    size_t m = 0, n = 0;
    if (fscanf(file, "%zu %zu", &m, &n) != 2) {
        fclose(file);
        return -3;
    }
    matrix->nrows = m;
    matrix->ncols = n;

    size_t total = m * n;
    matrix->data = malloc(total * sizeof(double));
    if (!matrix->data) {
        fclose(file);
        return -4;
    }

    for (size_t k = 0; k < total; ++k) {
        if (fscanf(file, "%lf", &matrix->data[k]) != 1) {
            free_matrix(matrix);
            fclose(file);
            return -5;
        }
    }

    fclose(file);
    return 0;
}

void print_line(const Matrix *matrix, size_t row, int many_cols) {
    size_t j;
    size_t l = many_cols ? 3 : matrix->ncols;
    size_t r = many_cols ? matrix->ncols - 3 : matrix->ncols;

    printf("[");
    for (j=0; j<l; j++) printf("%10.4lf%s", matrix->data[row + j*matrix->nrows], (j<matrix->ncols-1) ? " " : "");
    if (many_cols) printf("%10s ", (row==0 || row==matrix->nrows-1) ? "..." : "");
    for (j=r; j<matrix->ncols; j++) printf("%10.4lf%s", matrix->data[row + j*matrix->nrows], (j<matrix->ncols-1) ? " " : "");
    printf("]\n");
}

void print_sep(const Matrix *matrix, int many_cols) {
    size_t j;
    size_t num = many_cols ? 7 : matrix->ncols;

    printf("[");
    for (j=0; j<num; j++) {
        printf("%10s%s", (j==0 || j==num-1) ? "..." : "", (j<num-1) ? " " : "");
    }
    printf("]\n");
}

void print_matrix(const char *matrix_name, const Matrix *matrix) {
    if (!matrix || !matrix->data) return;
    if (matrix_name) printf("%s=\n", matrix_name);

    size_t i, t=matrix->nrows, b=matrix->nrows;
    int many_rows=0, many_cols=(matrix->ncols > 9);

    if (matrix->nrows > 9) {
        many_rows = 1;
        t = 3;
        b = matrix->nrows - 3;
    }

    for (i=0; i<t; i++) print_line(matrix, i, many_cols);
    if (many_rows) print_sep(matrix, many_cols);
    for (i=b; i<matrix->nrows; i++) print_line(matrix, i, many_cols);
}