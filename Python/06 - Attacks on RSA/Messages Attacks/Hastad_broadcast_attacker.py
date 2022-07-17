'''
Håstad's broadcast attack

Suppose one sender sends the same message M in encrypted form to a number of people P1, P2, ..., Pk,
each using the same small public exponent e and different moduli (ni, e)

If we have at least e messages encrypted (say, e = 3), we can reconstruct the original message.

Algorithm:

for all i < e:
    compute Ni = N // ni
    find Bezout coefficient ui and vi
the solution is:
    c = C1 * u1 * n1 + C2 * u2 * n2 + ... + Ce * ue * ne
'''

from Crypto.PublicKey import RSA

# Data
N_BITS = 1024
e = 3

def iroot (k, n):
    u ,s = n, n+1

    while u < s:
        s = u
        t = (k-1) * s + n // pow(s, k-1)
        u = t // k
    return s

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
    
    # Generating keys
    KEY_pri1 = RSA.generate(bits = N_BITS, e = e)
    KEY_pri2 = RSA.generate(bits = N_BITS, e = e)
    KEY_pri3 = RSA.generate(bits = N_BITS, e = e)
    
    # Retrieving parameters
    n1 = KEY_pri1.n
    n2 = KEY_pri2.n
    n3 = KEY_pri3.n
    N = n1 * n2 * n3
    
    # Encrypting message    
    message = b'This message is super confidential'
    x = int.from_bytes(message, 'big')
    C1 = pow(x, e, n1)
    C2 = pow(x, e, n2)
    C3 = pow(x, e, n3)
    
    # Attacking
    N1 = N // n1
    N2 = N // n2
    N3 = N // n3
    
    g, u1, v1 = egcd(N1, n1)
    g, u2, v2 = egcd(N2, n2)
    g, u3, v3 = egcd(N3, n3)

    x_recovered = iroot(e, C1 * u1 * N1 + C2 * u2 * N2 + C3 * u3 * N3)
    message_recovered = int.to_bytes(x_recovered, len(message), 'big')
    print('[!] Message recovered is: ', message_recovered.decode())
    