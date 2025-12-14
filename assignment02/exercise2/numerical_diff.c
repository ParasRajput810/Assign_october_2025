/**
 * @file numerical_diff.c
 * @brief Numerical differentiation using central difference and complex step.
 */

#include <stdio.h>
#include <math.h>
#include <complex.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

/**
 * @brief Function f(x) = exp(x) * (sin^3(x) + cos^3(x)).
 *
 * @param x Complex argument.
 * @return f(x) as a complex number.
 */
double complex cf(double complex x)
{
    double complex s = csin(x);
    double complex c = ccos(x);
    return cexp(x) * (s * s * s + c * c * c);
}

/**
 * @brief Central difference approximation of f'(x).
 *
 * Uses D1(h) = (f(x+h) - f(x-h)) / (2h).
 *
 * @param f Function pointer to f.
 * @param x Real point where derivative is approximated.
 * @param h Step size (real, positive).
 * @return Approximation of f'(x) as a real double.
 */
double central_diff(double complex (*f)(double complex), double x, double h)
{
    double complex fxph = f(x + h);
    double complex fxmh = f(x - h);
    double complex diff = (fxph - fxmh) / (2.0 * h);
    return creal(diff);
}

/**
 * @brief Complex step approximation of f'(x).
 *
 * Uses D2(h) = Im(f(x + i h)) / h.
 *
 * @param f Function pointer to f.
 * @param x Real point where derivative is approximated.
 * @param h Step size (real, positive).
 * @return Approximation of f'(x) as a real double.
 */
double complex_step(double complex (*f)(double complex), double x, double h)
{
    double complex fxih = f(x + I * h);
    return cimag(fxih) / h;
}

int main(void)
{
    const double x = PI / 4.0;
    const double fprime_exact = sqrt(2.0) * exp(PI / 4.0);

    FILE *fp = fopen("numerical_diff.txt", "w");
    if (!fp) {
        perror("fopen numerical_diff.txt");
        return 1;
    }

    fprintf(fp, "h | D_1(h) | error(D_1) | D_2(h) | error(D_2)\n");

    /* k = 1, 1.5, 2, 2.5, ..., 16 */
    for (double k = 1.0; k <= 16.0 + 1e-12; k += 0.5) {
        double h = pow(10.0, -k);

        double d1 = central_diff(cf, x, h);
        double d2 = complex_step(cf, x, h);

        double err1 = fabs(d1 - fprime_exact);
        double err2 = fabs(d2 - fprime_exact);

        fprintf(fp, "10^{-%g} | %.17e | %.17e | %.17e | %.17e\n",
                k, d1, err1, d2, err2);
    }

    fclose(fp);
    return 0;
}
