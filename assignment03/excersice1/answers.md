## Exercise 1(a): Mixed-precision iterative refinement

### Implementation summary

We implemented Algorithm 1 (mixed-precision iterative refinement) in C using BLAS/LAPACK:

- **High precision (double):**
  - Stored input matrix `A` and vectors `b`, `x` as `double`.
  - Computed the residual `r = b - A x` in **double** using `cblas_dgemv`.
  - Updated the solution `x = x + d` in **double**.
  - Checked the stopping criterion using the **double** 2-norm of the residual.

- **Low precision (float):**
  - Created a float copy of the matrix `A` (named `AsLU`) and a float copy of `b`.
  - Computed a one-time LU factorization of `AsLU` using `LAPACKE_sgetrf`.
  - Solved the initial system and each correction system `A d = r` in **float** using `LAPACKE_sgetrs`.

The inputs `A` and `b` are **not modified**, because all factorization work is done on a float copy of `A`, and `b` is only read.

### Stopping criterion

We used the required parameters:

- Tolerance: `tau = 1e-13`
- `maxiter > 10` (we use `maxiter = 200` in `main.c`)

The iteration stops when:

- `||r||_2 < tau`, where `r = b - A x` is computed in double precision.

### Required error metric (as given in the sheet)

The assignment asks for:

||x−˜x||∞ divided by ||x||∞

where `x` is the exact solution (vector of all ones) and `\tilde{x}` is the computed solution.
This is computed in `main.c` using the infinity norm.

### Results (dense_mat.txt, n = 100)

From running the program on the provided matrix:

- Iterations to converge: `5`
- Relative solution error: `||x_tilde - x||_2 / ||x||_2 = 4.8054524748988182e-09`
- Residual norm: `||b - A x_tilde||_2 = 4.6288679729439922e-14`
- Sheet metric: `||x - x_tilde||_inf / ||x||_inf = 1.2576776953210356e-08`

The residual norm is well below `1e-13`, so the solver converged successfully.

---

## Build prerequisites / project setup

### Required libraries

This project needs:

- A C compiler (e.g. `gcc`)
- **BLAS** implementation (we link with OpenBLAS)
- **LAPACK + LAPACKE** (C interface to LAPACK)
- `libm` (math library, linked via `-lm`)

On Debian/Ubuntu, install:

```bash
sudo apt update
sudo apt install -y build-essential libopenblas-dev liblapacke-dev
```
