#ifndef COO_H
#define COO_H

#include <stdio.h>

/**
 * @brief Coordinate (COO) sparse matrix container.
 */
typedef struct {
    int n_rows;       /**< Number of rows */
    int n_cols;       /**< Number of columns */
    int nnz;          /**< Number of nonzeros */
    int *row_indices; /**< Row indices of nonzeros (0-based) */
    int *col_indices; /**< Column indices of nonzeros (0-based) */
    double *values;   /**< Values of nonzeros */
} COOMatrix;

/**
 * @brief Read a matrix from a COO text file into @p A.
 * @param filename Path to file: first line "m n nnz", then lines "i j val" (0-based indices).
 * @param A Output struct; allocations inside must be freed with free_coo.
 * @return 0 on success, negative on error.
 * @code
 *   COOMatrix A = {0};
 *   if (read_coo("test_mat.txt", &A) != 0) {
 *       fprintf(stderr, "read_coo failed\n");
 *   }
 *   // use A...
 * @endcode
 */
int read_coo(const char *filename, COOMatrix *A);

/**
 * @brief Free memory owned by a COOMatrix allocated by read_coo.
 * @param A Matrix whose internal arrays will be freed; fields are reset.
 */
void free_coo(COOMatrix *A);

/**
 * @brief 2-norm of a length-n vector.
 * @param x Input vector.
 * @param n Length of vector.
 * @return 2-norm of x.
 */
double norm2(const double *x, int n);

/**
 * @brief Fill vector of length n with zeros.
 * @param x Output vector of all zeros.
 * @param n Length of vector.
 */
void zeros(double *x, int n);

/**
 * @brief Fill vector of length n with ones.
 * @param x Output vector of all ones.
 * @param n Length of vector.
 */
void ones(double *x, int n);

/**
 * @brief Sparse matrix-vector product y = A*x (in COO format).
 * @param A COO matrix.
 * @param x Input vector (length n_cols).
 * @param y Output vector (length n_rows).
 * @code
 *   double *x = ...; // length A.n_cols
 *   double *y = malloc((size_t)A.n_rows * sizeof(double));
 *   spmv(&A, x, y);
 *   // use y...
 * @endcode
 */
void spmv(const COOMatrix *A, const double *x, double *y);

/**
 * @brief Extract diagonal entries of A into @p diag (length n_rows).
 * @param A COO matrix.
 * @param diag Output vector containing the diagonal entries of A.
 */
void coo_extract_diag(const COOMatrix *A, double *diag);

#endif // COO_H
