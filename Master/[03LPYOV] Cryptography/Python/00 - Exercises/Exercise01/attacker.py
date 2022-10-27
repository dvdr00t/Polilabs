'''
LSB attack
'''

from decimal import getcontext, Decimal
from pwn import *

# Known information
HOST = 'localhost'
PORT = 12345
n = 84512936364028707109792721541348089559038960317411172574310460131821809228801
e = 65537


ciphertext = b';s\xf5\x8d\xd0\x8d\xeb\x02&A\xa4\xff\xd2\xc3%ht\x95\xe1)\xb2\xcc\xfc\xee\x18\x12}\x11)\xee\xec\xa6'

# Some useful data
getcontext().prec = n.bit_length()
sneakyC = int.from_bytes(ciphertext, 'big')
boundaries =  [Decimal(0), Decimal(n)]

for _ in range(n.bit_length()):
    
    # Crafting magical tools
    sneakyC = (pow(2, e, n) * sneakyC) % n
    
    # Opening connection
    conn = remote(HOST, PORT)
    
    # Sending data
    try: 
        conn.send(int.to_bytes(int(sneakyC), n.bit_length(), 'big'))
    except: 
        print('[!] Some errors sending data...')
        conn.close()
        break
    
    # Evaluating response
    response = conn.recv()
    if response == b'even': 
        boundaries[1] = (boundaries[0] + boundaries[1]) / 2 
    else: 
        boundaries[0] = (boundaries[0] + boundaries[1]) / 2 
    print(f'[{boundaries[0]}, {boundaries[1]}]')
    
    # Closing connection
    conn.close()
    
plaintext = int.to_bytes(int(boundaries[1]), n.bit_length(), 'big')
print('[!] Message recovered is: ', plaintext.decode().replace('\x00', ''))
    
    