'''
Trivial attack on two modulus (n1, n2) which have a common prime

Having: 
n1 = p1 * p2
n2 = p1 * p3

Then compute: 
p1 = gcd(n1, n2)
which is very fast to compute btw...

Finally:
p2 = n1 / p1
p3 = n2 / p1
'''

from Crypto.Util.number import getPrime
from math import gcd

# Data
N_BITS = 1024

if __name__ == '__main__':
    
    # Generating primes
    p1 = getPrime(N_BITS)
    p2 = getPrime(N_BITS)
    p3 = getPrime(N_BITS)
    primes = [p1, p2, p3]
    
    # Computing modulus
    n1 = p1 * p2
    n2 = p1 * p3
    
    # Computing gcd(n1, n2)
    p1_recovered = gcd(n1, n2)
    p2_recovered = n1 // p1
    p3_recovered = n2 // p1
    
    if p1_recovered in primes and p2_recovered in primes and p3_recovered in primes: 
        print('[-] p1_recovered: ', p1_recovered)
        print('[-] p2_recovered: ', p2_recovered)
        print('[-] p3_recovered: ', p3_recovered)
        print('[!] Recovering process completed!')
    else: 
        print('[!] Recovering process failed...')
        
        
    