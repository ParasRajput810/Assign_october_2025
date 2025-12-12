#include <stdio.h>

/* Direct evaluation: P(x) = sum_{i=0}^n a[i] * x^i
   Coefficients a are in ascending order: a[0] = a0, ..., a[n] = an */
double eval_poly(int n, double *a, double x) {
    double sum = 0.0;
    double xpow = 1.0;              /* x^0 initially */
    for (int i = 0; i <= n; ++i) {
        sum += a[i] * xpow;
        xpow *= x;
    }
    return sum;
}

/* Horner evaluation:
   P(x) = (...((a[n] * x + a[n-1]) * x + a[n-2]) ... ) * x + a[0] */
double eval_horner(int n, double *a, double x) {
    double y = a[n];
    for (int i = n - 1; i >= 0; --i) {
        y = y * x + a[i];
    }
    return y;
}

int main(void) {
    /* Polynomial (Exercise 2.b):
       P(x) = x^6 - 998x^5 - 998x^4 - 998x^3 - 998x^2 - 998x - 998 */
    int n = 6;
    double a[] = { -998.0, -998.0, -998.0, -998.0, -998.0, -998.0, 1.0 };

    double x1 = 1.0;
    double x2 = 999.0;

    double p1_direct = eval_poly(n, a, x1);
    double p1_horner = eval_horner(n, a, x1);

    double p2_direct = eval_poly(n, a, x2);
    double p2_horner = eval_horner(n, a, x2);

    printf("P(x) = x^6 - 998x^5 - 998x^4 - 998x^3 - 998x^2 - 998x - 998\n\n");

    printf("x = %.0f\n", x1);
    printf("  eval_poly   = %.17g\n", p1_direct);
    printf("  eval_horner = %.17g\n\n", p1_horner);

    printf("x = %.0f\n", x2);
    printf("  eval_poly   = %.17g\n", p2_direct);
    printf("  eval_horner = %.17g\n", p2_horner);

    return 0;
}