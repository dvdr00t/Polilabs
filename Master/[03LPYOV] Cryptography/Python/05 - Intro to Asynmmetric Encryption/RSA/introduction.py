'''
Performing RSA encryption "by hand"
-----------------------------------
Steps:

    1) Choose two large primes (p1, p2) of a given length (n_bits)
    2) Compute modulus: n = p1 * p2
    3) Compute Euler's Totient value: phi = (p1 - 1) * (p2 - 1)
    4) Check if gcd(phi, e) == 1
    5) Compute private key: d = pow(e, -1, phi)
    6) Compute encryption of x as: C = pow(x, e, n)
    7) Compute decryption of C as: x = pow(C, d, n)
'''

from math import gcd
from Crypto.Util.number import getPrime


if __name__ == '__main__':
    
    # Hardcoded information
    n_bits = 1024
    e = 65537
    
    # generating primes
    p1 = getPrime(n_bits)
    p2 = getPrime(n_bits)
    
    # computing modulus
    n = p1 * p2
    
    # computing phi
    phi = (p1 - 1) * (p2 - 1)
    if gcd(e, phi) != 1:
        raise ValueError
    
    # computing private exponent
    d = pow(e, -1, phi)
    
    # ----------------------------
    
    # ENCRYPTION
    message = b'This is a super secure message!'
    x = int.from_bytes(message, byteorder='little')
    if x > n - 1:
        raise ValueError
    print('[!] Message (int) is: ', x)
    C = pow(x, e, n)
    
    # DECRYPTION
    D = pow(C, d, n)
    message = int.to_bytes(D, length=len(message), byteorder='little')
    print('[!] Message is: ', message.decode())
    