# EXERCISE 08
## Code implementation with threads

In linear algebra, the multiplication of matrices is the operation that
produces a new matrix $C$ by making the product rows for columns of two
given matrices $A$ and $B$.
More in detail, if $A$ has size $[r, x]$ and $B$ has size $[x, c]$, then $C$ will
have size $[r, c]$, and each of its own position elements $(i, j)$ will be
computed as:

$$C[i][j] = \sum_{k=0}^{x-1} A[i][k] x B[k][j]$$

Write a multithreaded function

```C
void mat_mul (int **A, int **B, int r, int x, int c, int **C);
```

able to generate the matrix $C$, running a thread to compute
each one of its elements.
Each thread will calculate the value of the element, making the product
rows by columns previously specified.
Properly define the data structure required to run the threads.
