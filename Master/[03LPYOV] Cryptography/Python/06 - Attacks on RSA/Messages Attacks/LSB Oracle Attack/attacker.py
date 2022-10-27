'''
LSB Oracle Attack
Server is a oracle which leaks least significant bit after decrypting the given ciphertext.

----
Example when to use:

    - Oracle server has API which decrypts a ciphertext and response with LSB
    - Oracle server has API which decrypts a ciphertext and response true/false based on the fact that the plaintext is even/odd
    
----

We know that:

    C = m^e mod n   with m < n

we send 

    C' = 2^e * C = 2^e * m^e mod n = (2m) ^ 2 mod n
'''

from email import message
from pwn import *


# Known information
HOST = 'localhost'
PORT = 12345
CIPHERTEXT = 40905797042890600077330500098053021483209678644028914795144404253281221960366
n = 84512936364028707109792721541348089559038960317411172574310460131821809228801
e = 65537

# Utilities
upper_bound = n
lower_bound = 0
sneakyC = CIPHERTEXT

if __name__ == '__main__':
    
    for i in range (n.bit_length()):
        
        # Crafting sneaky items
        sneakyC = (pow(2, e, n) * sneakyC) % n
        # sneakyC = (pow(2, e*i, n) * CIPHERTEXT) % n
    
        # Opening connection 
        conn = remote(HOST, PORT)
    
        # Sending data
        try:
            conn.send(int.to_bytes(sneakyC, length=n.bit_length(), byteorder='big'))
        except:
            print('ops...')
            conn.close()
    
        # Receiving response
        lsb = conn.recv()
        
        # Evaluating LSB
        if lsb[0] == 0:
            upper_bound = (upper_bound + lower_bound) // 2
        else:
            lower_bound = (upper_bound + lower_bound) // 2
        print(f'[{lower_bound}, {upper_bound}]')
    
        # Closing connection
        conn.close()
    
    message_recovered = int.to_bytes(lower_bound, n.bit_length(), 'big')
    print('[!] Message recovered: ', message_recovered.decode())
    
    
