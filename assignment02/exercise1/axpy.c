/* axpy.c */

#include "axpy.h"

void axpy(int n, double alpha, double *x, double *y, double *z)
{
    for (int i = 0; i < n; ++i) {
        /* Read inputs into temporaries first to be safe for in-place usage. */
        double xi = x[i];
        double yi = y[i];
        z[i] = alpha * xi + yi;
    }
}
