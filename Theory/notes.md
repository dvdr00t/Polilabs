# Cryptography 
### A.Y. 2021-2022
---

# Finding the Last Digit of a Power
### Recognizing pattern

A common way to attack these type of questions is to list out the initial expansions of a power to determine a pattern. Questions which ask about the last decimal digit of a power can be solved completely after proving that the pattern in question holds (often by induction). The last digits of various powers of an integer are given in the table below:

$$
\begin{array}{c|cccc}
\text { digit } d & d^{2} & d^{3} & d^{4} & d^{5} \\
\hline 1 & 1 & 1 & 1 & 1 \\
2 & 4 & 8 & 6 & 2 \\
3 & 9 & 7 & 1 & 3 \\
4 & 6 & 4 & 6 & 4 \\
5 & 5 & 5 & 5 & 5 \\
6 & 6 & 6 & 6 & 6 \\
7 & 9 & 3 & 1 & 7 \\
8 & 4 & 2 & 6 & 8 \\
9 & 1 & 9 & 1 & 9
\end{array}
$$

From the table, we can see the following:

$$
\begin{array}{|c|c|}
\hline \text { The last digit of powers of } 1 \text { is always } & 1 \\
\hline \text { The last digits of powers of } 2 \text { repeat in a cycle of } & 6,2,4,8 \\
\hline \text { The last digits of powers of } 3 \text { repeat in a cycle of } & 1,3,9,7 \\
\hline \text { The last digits of powers of } 4 \text { repeat in a cycle of } & 6,4 \\
\hline \text { The last digit of powers of } 5 \text { is always } & 5 \\
\hline \text { The last digit of powers of } 6 \text { is always } & 6 \\
\hline \text { The last digits of powers of } 7 \text { repeat in a cycle of } & 1,7,9,3 \\
\hline \text { The last digits of powers of } 8 \text { repeat in a cycle of } & 6,8,4,2 \\
\hline \text { The last digits of powers of } 9 \text { repeat in a cycle of } & 1,9 \\
\hline
\end{array}
$$

### Chinese Remainder Theorem
The Chinese remainder theorem is a powerful tool to find the last few digits of a power. The idea is to find a number mod $5^n$ and mod $2^n$ and then combine those results, using the Chinese remainder theorem, to find that number mod $10^n$.

- **Example:**
Find the last two digits of $74^{540}$.
Observe that $100=4 \times 25$ and $\operatorname{gcd}(4,25)=1$. So we can compute $74^{540}(\bmod 4)$ and $74^{540}$ (mod 25$)$, and then combine those results to find $74^{540} \bmod 100$. Now
$$
\begin{array}
74^{540} \equiv 2^{540} \times 37^{540} \equiv 0 \quad(\bmod 4) \\
74^{540} \equiv(-1)^{540} \equiv 1 &(\bmod 25)
\end{array}
$$
The unique solution $\bmod 100$ to $x \equiv 0 (\bmod 4)$ and $x \equiv 1(\bmod 25)$ is 76 , so this is the answer.
