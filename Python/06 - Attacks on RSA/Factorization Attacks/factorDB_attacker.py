'''
factorDB can be used to factorize a small number
Apparently only < 30 bits
'''

from Crypto.Util.number import getPrime
from factordb.factordb import FactorDB

# Data
N_BITS = 30

if __name__ == '__main__':
    
    # Generating primes
    p1 = getPrime(N_BITS)
    p2 = getPrime(N_BITS)
    
    # Computing modulus
    n = p1 * p2
    
    # Recovering primes (simulating to only have n)
    f = FactorDB(n)
    f.connect()
    primes = f.get_factor_list()
    
    # Checking equality
    if p1 in primes and p2 in primes:
        print('Recovering process completed!')
    else:
        print('Recovering process failed...')
