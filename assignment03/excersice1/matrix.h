#ifndef SC_MATRIX_H
#define SC_MATRIX_H

#include <stdlib.h>

/**
 * \brief Matrix struct containing its dimensions and data
 *
 * \note Matrix is in column-major order
 */
typedef struct Matrix {
    size_t nrows, ncols; // matrix dimensions
    double *data;        // matrix entries in column-major order
} Matrix;

/**
 * \brief Frees up the memory of the data
 * \param[in] matrix struct of the matrix which is to be freed
 *
 * \note Does not free the struct itself, since it might be on the stack
 */
void free_matrix(Matrix *matrix);

/**
 * \brief Parses a file and stored the data inside the matrix
 * \param [in,out] matrix struct of the matrix into which the data is parsed
 * \return returncode of 0 on success and not zero on failure to parse
 *
 * \note Matrix is parsed in column-major order
 *
 * Example usage:
 * \code
 * char *some_path = "...";
 * Matrix A; // create on stack
 * if (parse_matrix(some_path, &A)) {
 *     // do error management
 * }
 * \endcode
 */
int parse_matrix(const char *filename, Matrix *matrix);

/**
 * \brief Prints a matrix to \c stdout
 * \param[in] matrix_name optional matrix name, which is printed beforehand
 * \param[in] matrix struct of the matrix which is to be printed
 */
void print_matrix(const char *matrix_name, const Matrix *matrix);

#endif // SC_MATRIX_H