# Exercise 2: Sparse Matrix Solvers

## Task 2(a): Basic Utilities in COO Format

### Implementation

Implemented the following functions in `coo.c`:

- `spmv`: sparse matrix-vector product `y = A*x` for COO matrices (accumulates contributions)
- `coo_extract_diag`: extracts the diagonal entries into a dense vector `diag`
- `norm2`: Euclidean (L2) norm, `||x||_2`
- `zeros`: fills a vector with zeros
- `ones`: fills a vector with ones

All routines are written defensively (basic null checks) and work for general COO data.

### Testing results

Using the provided test matrix `test_mat.txt` (`3000×3000`, `nnz = 9000`):

**Vector of ones (`x`):**

- First 5 elements: `[1, 1, 1, 1, 1]`
- L2 norm:
  \[
  \|x\|\_2 = \sqrt{3000} \approx 54.7723
  \]
  Program output: `||x||_2 = 5.4772255750516614e+01` (matches)

**Sparse matrix-vector product (`y = A*x`):**

- First 5 elements: `[26.027469, 25.860489, 25.847819, 2.602747, 2.586049]`
- L2 norm: `||A*x||_2 = 1.0085749475023382e+03`

**Diagonal extraction (`diag(A)`):**

- First 5 diagonal values: `[10, 10, 10, 1, 1]`
- L2 norm: `||diag(A)||_2 = 3.8923000912057125e+02`

### Memory management

Valgrind check:

- `in use at exit: 0 bytes in 0 blocks`
- `All heap blocks were freed -- no leaks are possible`
- `ERROR SUMMARY: 0 errors from 0 contexts`

So there are no memory leaks and no invalid memory accesses.

---

## Task 2(b): Weighted Jacobi

We implemented the weighted Jacobi iteration
\[
x\_{k+1} = x_k + \omega D^{-1}(b - A x_k),
\]
with initial guess `x0 = 0`, right-hand side `b = 1`, tolerance `||r||_2 < 1e-6`, and maximum iterations `maxiter = 5000`, where the residual is
\[
r_k = b - A x_k.
\]

### Results on `test_mat.txt`

- \(\omega = 1/3\): converged in **175** iterations, final \(||r||\_2 = 9.3747970471996063\times 10^{-7}\)
- \(\omega = 2/3\): converged in **84** iterations, final \(||r||\_2 = 9.8482514166010899\times 10^{-7}\)
- \(\omega = 1\): did **not** converge; the iteration diverged (residual overflowed to `inf`). The solver detects this and stops with failure code `-7`.

### Discussion

The relaxation parameter \(\omega\) changes the iteration matrix and therefore the convergence behavior. For this matrix, \(\omega = 2/3\) converges significantly faster than \(\omega = 1/3\), while \(\omega = 1\) is unstable and diverges. Therefore, an under-relaxed choice \(\omega < 1\) is necessary here for convergence.
