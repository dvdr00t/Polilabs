from pwn import *
from Crypto.Cipher import AES

# DATA
HOST = 'localhost'
PORT = 12346

'''
            BIT-FLIPPING ATTACK
-------------------------------------------

Server request for an input and perform the following operation: 
cookie = b'username='+username+b',admin=0'

We know (by inspecting the code) that it is using AES in CBC mode. The size of a block in AES
is 16 bytes. We have control over the username which will start occupy the first block

string (block):     | username=AAAAAAA | ,admin=0________ |     (where ____ is just pad)

Sending username == 'AAAAAAA' gets as a response from the server an encrypted cookie, whose
first block is the following:
b'\x00\xbe/\xaa\n\xdcQ\x10O\xcd\xf5\xc2\x17\xb8\x1as\x9e\xfa'\xa2\xd6V\xc2\xf6\xc4\x14\xe3\xb1\xdb\x84=='
b'\x00\xbe/\xaa\n\xdcQ\x10O\xcd\xf5\xc2\x17\xb8\x1as' <-- First block
b'\x9e\xfa'\xa2\xd6V\xc2\xf6\xc4\x14\xe3\xb1\xdb\x84==' <-- Second block
                      ^
            Admin value is stored here (index == 23)
            
Since we are in CBC, we need to actually change the previous block of the encryption to apply the modification to 
the current block.

We know that the modification propagates with a XOR operation, so we simply need to create a mask:

                            ord(b'0') ^ ord(b'1')
                            
that will be applied to the ciphertext.


+ ------------- +                   + ------------- +  
|      y0       |                   |      y1       | 
+ ------------- +                   + ------------- +
        |                                   |
        | -------------- +                  |
      |dec|              |                |dec|
        |                |                  |
        |                |                  |   Here we apply the mask coming from y0
IV -->|XOR|              + -------------> |XOR| in order to change the specific bit in x1
        |                                   |
        |                                   |
        x0                                  x1



'''

# Opening connection
conn = remote(HOST, PORT)

# Exploiter
username = b'AAAAAAA'
target = b'username='+username+b',admin=0'
index = target.index(b'0')
mask = ord(b'1') ^ ord(b'0')

# Send the username
try:
    conn.send(username)
except:
    print("Error")
cookie = conn.recv()
cookie = list(cookie)
cookie[index-AES.block_size] ^= mask
cookie = bytes(cookie)

# Send back the cookie
try:
    conn.send(cookie)
except:
    print("Error")
response = conn.recv()
print(response)

# Closing connection
conn.close()

    


