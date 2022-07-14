from Crypto.Cipher import AES
from pwn import *

# Configuration data
HOST = '127.0.0.1'
PORT = 12345

# Discovered information
prefix = b'Here is the message:'
postfix = b' - and the secret is:'

'''
At the first iteration, the server will encrypt the following message:

block (number):            0                   1                  2                  3                  4                  5                 6                   7
message (block):    Here is the mess | age:AAAAAAAAAAAA | AAAAAAAAAAAAAAAA | AAAAAAAAAA - and |  the secret is:_ | AAAAAAAAAAAAAAAA | AAAAAAAAAA - and |  the secret is:_ | ________
                                                                                                               ^                                                        ^
                                                                                                        Here is our guess                                       Here is secret[0]   

[NB: we add two padding blocks 2 and 5 because we need to shift block 6-7 (secret) without messing up with our block 3-4 (guess)]
The server responses with the encrypted version of the message.
If our guess encrypted is equal to secret[0] encrypted, we have leaked the first character of the secret message.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

At the second iteration, the server will encrypt the following message:

block (number):            0                   1                  2                  3                  4                  5                 6                   7
message (block):    Here is the mess | age:AAAAAAAAAAAA | AAAAAAAAAAAAAAAA | AAAAAAAAA - and  | the secret is:H_ | AAAAAAAAAAAAAAAA | AAAAAAAAA - and  | the secret is:__ | ________
                                                                                                               ^                                                        ^
                                                                                                        Here is our guess                                       Here is secret[1] 

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

When we shift an entire block: 
                                                                                                        
block (number):            0                   1                  2                  3                  4                  5                 6                   7
message (block):    Here is the mess | age:AAAAAAAAAAAA | AAAAAAAAAAAAAAAA | AAA - and the se | cret is:Here's _ | AAAAAAAAAAAAAAAA | AAA - and the se | cret is:________ | ________
                                                                                                               ^                                                        ^
                                                                                                        Here is our guess                                       Here is secret[1] 

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

In general, at every iteration:
    - We guess last byte of block 4 to match last byte of block 7
    - We remove one byte from block 2-3-4 to provide space to shift block 5-6-7
'''

MAX_LENGTH = AES.block_size+10
secret = b''
block234 = b'A'*AES.block_size + b'A'*10 + postfix  # | AAAAAAAAAAAAAAAA | AAAAAAAAAA - and |  the secret is:  |  ===  "AAAAAAAAAAAAAAAAAAAAAAAAAA - and the secret is:"
for i in range(MAX_LENGTH):
    for guess in string.printable:
        
        # Opening connection
        conn = remote(HOST, PORT)
        
        # Crafting input
        input = b'A'*12 + block234[i:] + secret + guess.encode() + b'A'*((AES.block_size+10) - i)
        
        # Sending data
        try:
            conn.send(input)
        except:
            print('Ops... Something went wrong...')
        
        # Receiving reponse
        response = conn.recv()
        
        if response[64:64+AES.block_size] == response[112:112+AES.block_size]:
            secret += guess.encode()
            break 
        
        # Closing connection
        conn.close()
    
    print(secret)