'''
CBC Padding Oracle Attack
'''

from Crypto.Random import get_random_bytes
from Crypto.Cipher import AES
from pwn import *

# Known information
HOST = 'localhost'
PORT = 12345
cbc_oracle_iv = b'\xd9H\xaf\xc9\xa5\xc9"3\x93\xaa\xbd\x87\xa5\x15\x04\xdd'
cbc_oracle_ciphertext = b"\x94\x99d{SE,\x00I|\x04\xfe\x14N5\xb4\x1c\xadk\x9f\xfc\xceg8'\xe6{\x13\xc9\x94\xce\xae\x0b\x97\xfbm\x8b0\n\\\x94-~\xa7CH\xbf\x150\x88:(\x8c\xeb\xbf5\xaf\xdd_5\xe9\xb4f\xf9"

def get_n_block(text: bytes) -> int:
    """ Given a bytes string (multiple of 16), it returns the number of blocks.
            - Note that if the text is not a multiple of 16 it will trigger
              an exception. 
    """
    
    # Checking length    
    if len(text) % AES.block_size != 0:
        raise Exception('Text size is not a multiple of a block size')
    
    # Counting
    return len(text) // AES.block_size
    

def get_nth_block(n: int, text: bytes) -> bytes:
    """ Given a bytes string (multiple of 16), it returns the n-th block.
            - First block is block 0
            - Note that if the text is not a multiple of 16 it will trigger
              an exception. 
    """
    
    # Checking length    
    if len(text) % AES.block_size != 0:
        raise Exception('Text size is not a multiple of a block size')
    
    # Returning block
    return text[(n*AES.block_size):((n+1)*AES.block_size)]

def guess_block(c1: bytes, c2: bytes) -> bytes:
    
    '''
    LINK: https://robertheaton.com/2013/07/29/padding-oracle-attack/

    Decrypt a specific block c2 to p2 given the previous block c1.
    
    + ------------- +                   + ------------- +  
    |      c1       |                   |      c2       | 
    + ------------- +                   + ------------- +
            |                                   |
            |                                   |
            + --------- +                  + ------- +
                        |                  |   dec   |
                        |                  + ------- +
                        |                       |
                        |               + ------------- +
                        |               |      i2       |   Intermediate block right after decryption but
                        |               + ------------- +   before XOR
                        |                       |
                        |                       |
                        |                  + ------- +
                        + -------------- > |   XOR   |
                                           + ------- +
                                                |
                                                |
                                        + ------------- +
                                        |      p2       |
                                        + ------------- +
                                        
    
    If we want to decrypt c2, we need to craft a specific c1 (called c1_magic in the code) that allows
    us to discover information about p2 thanks to the oracle.
    
    Remember that we can pass in any ciphertext, and the server will tell us whether it decrypts to plaintext 
    with valid padding or not. We exploit this by passing in c1_magic + c2, where c1_magic is a sneakily chosen 
    ciphertext block, c2 is the ciphertext block we are trying to decrypt. 
    The decrypted plaintext block produced will be simple pad if we guess the byte correctly.
    
    Padding style is X9.23:
    
    > | __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ 01 |
    > | __ __ __ __ __ __ __ __ __ __ __ __ __ __ 00 02 |
    > | __ __ __ __ __ __ __ __ __ __ __ __ __ 00 00 03 |
    > | __ __ __ __ __ __ __ __ __ __ __ __ 00 00 00 04 |
    > | __ __ __ __ __ __ __ __ __ __ __ 00 00 00 00 05 |
    > ...
    > etc...
    
    Formulas:

        byte 15: 
                        i2[15] = iteration XOR c1_magic[15]      // c1_magic[15] will be a guess in range(256)
                        p2[15] = i2[15] XOR c1[15]          // because i2[15] will be the same, is the right value
                        
        byte 14:
                        c1_magic[15] = i2[15] XOR 0x02      // we need the actual value that produces the pad
                        i2[14] = 0x00 XOR c1_magic[14]      // c1_magic[14] will be a guess in range(256) 
                        p2[14] = i2[14] XOR c1[14]          // because i2[14] will be the same, is the right value              
                        
        byte 13:
                        c1_magic[14] = i2[14] XOR 0x03      // we need the actual value that produces the pad
                        i2[13] = 0x00 XOR c1_magic[13]      // c1_magic[13] will be a guess in range(256) 
                        p2[13] = i2[13] XOR c1[13]          // because i2[13] will be the same, is the right value 
        
        byte 12:
                        c1_magic[13] = i2[13] XOR 0x04      // we need the actual value that produces the pad
                        i2[12] = 0x00 XOR c1_magic[12]      // c1_magic[12] will be a guess in range(256) 
                        p2[12] = i2[12] XOR c1[12]          // because i2[12] will be the same, is the right value  
                        
        and so on... 
    '''
    
    # Utilities
    i2 = list(get_random_bytes(AES.block_size))     # Intermediate value after decryption before XOR
    p2 = list(b' '*AES.block_size)                  # Actual plaintext
    
    # Guessing all the block
    for i in range(AES.block_size):
        
        # Crafting customized block to perform the attack
        c1_magic = get_random_bytes(AES.block_size - i - 1) + b' '*(i+1)
        
        # Adding previously discovered information
        c1_magic = list(c1_magic)
        for j in range(i):
            if j == i-1:
                c1_magic[AES.block_size-1] = (i+1) ^ i2[AES.block_size-1]
            else: 
                c1_magic[AES.block_size-j-2] = 0 ^ i2[AES.block_size-j-2]
        c1_magic = bytes(c1_magic)
        
        # Trying all possible values for a byte
        for guess in range(2**8):
            c1_magic = list(c1_magic)
            c1_magic[AES.block_size-i-1] = guess
            c1_magic = bytes(c1_magic)

            # Sending data
            conn = remote(HOST, PORT)
            try:
                conn.send(cbc_oracle_iv)
                conn.send(c1_magic + c2)
            except:
                print("Error")
            response = conn.recv()
            conn.close()
            
            # Checking response
            if response == b'1':
                if i == 0:
                    i2[AES.block_size-1-i] = guess ^ 1
                    p2[AES.block_size-1-i] = c1[AES.block_size-1-i] ^ i2[AES.block_size-1-i]
                    print(bytes(p2))
                    break
                i2[AES.block_size-1-i] = guess ^ 0
                p2[AES.block_size-1-i] = c1[AES.block_size-1-i] ^ i2[AES.block_size-1-i]
                print(bytes(p2))
                break
    
    return bytes(p2)

if __name__ == '__main__':
    
    # Retrieving number of blocks
    nblocks = get_n_block(cbc_oracle_ciphertext)
    p = []
    
    # Decrypting ciphertext[16:]
    for b in range(nblocks-1):
        c1 = get_nth_block(nblocks-2-b, cbc_oracle_ciphertext)
        c2 = get_nth_block(nblocks-1-b, cbc_oracle_ciphertext)
    
        p.append(guess_block(c1, c2))
        
    # Decrypting first block of ciphertext, based on IV
    p.append(guess_block(cbc_oracle_iv, get_nth_block(0, cbc_oracle_ciphertext)))
    
    # Printing result
    print('[!] Decryption completed: ', end='')
    print(b''.join(reversed(p)))
