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

## Trucco magico

18^23 mod 55 = 2 mod 55 —> con CRT?

Nope

C'è un trucco op di @daveinnocenzi. Scrivete il numero in binario:

18^(10111)  e C0=1

E poi scrivete:

C0^2 * 18^1 = C1
C1^2 * 18^0 = C2
C2^2 * 18*1 = C3
C3^2 * 18^1 = C4
C4^2 * 18^1 = C5

C5 è il risultato
