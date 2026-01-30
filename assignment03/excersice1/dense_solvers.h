#ifndef DENSE_SOLVERS_H
#define DENSE_SOLVERS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Stores the number of iterations performed in the last mp_iter_refinement call. */
extern int mp_last_iterations;

int mp_iter_refinement(int n, double *A, double *b, double *x, int maxiter);

#ifdef __cplusplus
}
#endif

#endif