from pwn import *
from Crypto.Cipher import AES

# Configuration data
HOST = '127.0.0.1'
PORT = 12345

# Opening remote connection to the server
conn = remote(HOST, PORT)

# Crafting customized message
message = b'A' * 3 * AES.block_size
conn.send(message)

# Receiving response from server
ciphertext = conn.recv()
conn.close()

'''
Checking if server is using CBC or ECB
Message sent is something like:

   size (byte):              n_pre              16                 16             n_post
message (blocks):       __prefix__AAAA   AAAAAAAAAAAAAAAA   AAAAAAAAAAAAAAAA   AAAA__postfix__ 

Therefore is server is using ECB, it can be detected by checking if encryption of
second and third block is the same.

Since we don't know the length of the __prefix__ and __postfix__ part, it is necessary to send at least
3 * AES.block_size in order to obtain at least 2 complete block of the same plaintext.
'''

# Printing ciphertext
for i in range(len(ciphertext) // AES.block_size):
    print(ciphertext[(i*AES.block_size):((i+1)*AES.block_size)])
    
# checking equality
if ciphertext[2*AES.block_size:3*AES.block_size] == ciphertext[3*AES.block_size:4*AES.block_size]:
    print("Server is using ECB!!!")
else:
    print("Server is using CBC!!!")



