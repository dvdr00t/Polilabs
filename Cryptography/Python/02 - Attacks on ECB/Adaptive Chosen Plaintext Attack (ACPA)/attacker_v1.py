from Crypto.Cipher import AES
from pwn import *

# Configuration data
HOST = '127.0.0.1'
PORT = 12345

# Discovered information
prefix = b'Here is the msg:' # 16 Bytes --> 1 block
postfix = b' - and the sec:' # 15 Bytes --> will required some alignment


'''
At the first iteration, the server will encrypt the following message:

block (number):            0                   1                  2                  3
message (block):    Here is the msg: |  - and the sec:_ | AAAAAAAAAAAAAAAA |  - and the sec:_ | ________
                                                      ^                                     ^
                                             Here is our guess                      Here is secret[0]   

[NB: we add a second padding block because we need to shift block 3 without messing up with our block 1]
The server responses with the encrypted version of the message.
If our guess encrypted is equal to secret[0] encrypted, we have leaked the first character of the secret message.


At the second iteration, the server will encrypt the following message:

block (number):            0                   1                 2                 3
message (block):    Here is the msg: | - and the sec:H_ | AAAAAAAAAAAAAAA  | - and the sec:__ | ________


In general, at every iteration:
    - We guess last byte of block 1 to match last byte of block 3
    - We remove one byte from block 2 to provide space to shift block 3
'''

MAX_LENGTH = 16
secret = b''
for i in range(MAX_LENGTH):
    for guess in string.printable:
        
        # Opening connection
        conn = remote(HOST, PORT)
        
        # Crafting input
        input = postfix[i:] + secret + guess.encode() + b'A'*(AES.block_size-i)
        # print(input)
        
        # Sending data
        try:
            conn.send(input)
        except:
            print('Ops... Something went wrong...')
        
        # Receiving reponse
        response = conn.recv()
        
        if response[31] == response[63]:
            secret += guess.encode()
            break 
        
        # Closing connection
        conn.close()
    
    print(secret)