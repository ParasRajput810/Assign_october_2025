#ifndef SOLVERS_H
#define SOLVERS_H

#include "coo.h"

/**
 * @brief Weighted Jacobi iteration for Ax=b (COO matrix).
 *
 * Uses the iteration:
 *   x_{k+1} = x_k + omega * D^{-1} (b - A x_k)
 *
 * Stops when ||r||_2 < 1e-6, with r = b - A x_k, or when maxiter=5000 is reached.
 *
 * @param A COO matrix (assumed square for this task).
 * @param b RHS vector (length n).
 * @param omega Relaxation parameter in (0,1].
 * @param x In/out: initial guess on input, solution estimate on output (length n).
 * @param res Output: final residual norm ||r||_2.
 * @return number of iterations used to reach tolerance, or 5000 if not converged,
 *         or negative value on failure (e.g., zero diagonal entry).
 */
int w_jacobi(const COOMatrix *A, const double *b, double omega, double *x, double *res);

#endif // SOLVERS_H