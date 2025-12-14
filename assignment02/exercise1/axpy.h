/* axpy.h */

#ifndef AXPY_H
#define AXPY_H

/* Compute z := alpha * x + y for vectors of length n.
 * x, y, z may point to the same memory (in-place operation is allowed).
 */
void axpy(int n, double alpha, double *x, double *y, double *z);

#endif /* AXPY_H */
