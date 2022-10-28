'''
Low Public Exponent (LPE) Attack

If the public exponent is 3 (e = 3), than we can exploit the fact that, probably, cubing the message will 
not overflow the modulus n. Therefore, the message will just be the cubic root of the ciphertext.

e = 3
C = m^3 mod n       // where m * m * m < n
m = C^(1/3)
'''

from Crypto.PublicKey import RSA

# Data
N_BITS = 2048
e = 3

def iroot (k: int, n: int) -> int:
    u ,s = n, n+1

    while u < s:
        s = u
        t = (k-1) * s + n // pow(s, k-1)
        u = t // k
    return s

if __name__ == '__main__':
    
    # Generating RSA key
    KEY_pri = RSA.generate(bits = N_BITS, e = e)
    n = KEY_pri.n
    
    # Encrypting the message
    message = b'Honestly, a not so short message can be recovered here...'
    x = int.from_bytes(message, 'big')
    C = pow(x, e, n)
    
    # Exploiting LPE
    x_recovered = iroot(e, C)
    message_recovered = int.to_bytes(x_recovered, len(message), 'big')
    print('[!] Message recovered is: ', message_recovered.decode())
    