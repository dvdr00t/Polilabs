'''
Attempt factorization of large numbers with Fermat
'''

from Crypto.Util.number import getPrime
from gmpy2 import isqrt

# Data
N_BITS = 32

def fermat(N: int) -> tuple:
    
    '''
    Based on the fact that, if factors are close to each other, we can write:
    
            N = p * q = (a + b) * (a - b) = a^2 + b^2
            
    '''
    
    # Assuring n to be odd
    if N % 2 == 0:
        raise ValueError
    
    print('[.] initialization ...')
    
    # Algorithm implementation
    a = b = isqrt(N)                # integer square of n
    b2 = pow(a,2) - N
    
    counter = 0
    print('[.] starting iteration ...')
    while True:
        print(f'     [>] iteration {counter+1}')
        if b2 == pow(b, 2):
            print(f'[!] Found: a is {a} and b is {b}')
            break
        else:
            a += 1
            b2 = pow(a, 2) - N
            b = isqrt(b2)
        
        counter += 1
        
    return (a+b, a-b)


if __name__ == '__main__':
    
    # Generating two random primes
    p1 = getPrime(N_BITS)
    p2 = getPrime(N_BITS)
    primes = []
    primes.append(p1)
    primes.append(p2)
    
    print(f'[-] p1 is: {p1}')
    print(f'[-] p1 is: {p2}')
    n = p1 * p2
    print(f'[-] n is: {n}')
    
    p1_recovered, p2_recovered = fermat(n)
    if p1_recovered in primes and p2_recovered in primes:
        print(f'[-] p1_recovered is: {p1_recovered}')
        print(f'[-] p2_recovered is: {p2_recovered}')
        print('[!] Factorization completed successfully!')
    else:
        print('[!] Factorization failed...')
    
    