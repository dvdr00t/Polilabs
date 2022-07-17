'''
Suppose to have two public keys:

    KEY_pub1 = (e1, n)
    KEY_pub2 = (e2, n)
    
which are used to encrypt the same message m:

    C1 = m^e1 mod n
    C2 = m^e2 mod n
    
With Bezout it is possible to find:

    ue^1 + ve^2 = gcd(e1, e2)
    
If gcd(e1, e2) == 1 it is easy to recover the message m as:

    m = C1^u * C2^v mod n
'''

from math import gcd
from Crypto.Util.number import getPrime

# Data
N_BITS = 1024
e1 = 65537
e2 = 17


def egcd(a: int, b: int) -> tuple:
    '''
    Computes Bezout gcd()
    '''
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)

if __name__ == '__main__':
    
    while True:
        # Generate primes
        p1 = getPrime(N_BITS)
        p2 = getPrime(N_BITS) 
        n = p1 * p2
        print('[O] Generated primes')
        
        # Compute totient
        phi = (p1 - 1) * (p2 - 1)
        if gcd(e1, phi) == 1 and gcd(e2, phi) == 1:
            print('[O] Computed totient')
            break    
    
    # Compute private exponents
    d1 = pow(e1, -1, phi)
    d2 = pow(e2, -1, phi)
    print('[O] Computed private exponents')
    
    # Encrypting message
    message = b'This message is super confidential!'
    x = int.from_bytes(message, 'big')
    
    C1 = pow(x, e1, n)
    C2 = pow(x, e2, n)
    print('[O] Encrypted message')
    
    # Exploiting common modulus
    res = egcd(e1,e2)
    u = res[1]
    v = res[2]
    
    if res[0] == 1:
        
        # m = C1^u * C2^v mod n
        x_recovered = (pow(C1, u, n) * pow(C2, v, n)) % n
        message_recovered = int.to_bytes(x_recovered, len(message), 'big')
        print('[!] Recovered message is: ', message_recovered.decode())
    
    
    
    