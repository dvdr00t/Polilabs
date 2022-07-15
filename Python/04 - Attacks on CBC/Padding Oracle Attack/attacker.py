from Crypto.Random import get_random_bytes
from Crypto.Cipher import AES
from pwn import *

# Data discovered to perform the attack
HOST = 'localhost'
PORT = 12345
ciphertext = b'r\x8b\x14\xd6\xae{J\xa0\xe3\x9e\n\x96>\xf9=c\x0f\x16\x9at\x80\ny\xcfD\x08\xe7\xbe\xc1\x8d\x06U\x17J\xb4.\xe1\x9b48R\x8d\xd7\x04\xad\x0b\x7f\xbc\x82\xb0d2\x0bm\xc7\xdeX)\xb2\x7f\xb2\xe7I\x14'
IV = b'\xd9H\xaf\xc9\xa5\xc9"3\x93\xaa\xbd\x87\xa5\x15\x04\xdd'

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
    
    Formulas:

        byte 15: 
                        i2[15] = 0x01 XOR c1_magic[15]      // c1_magic[15] will be a guess in range(256)
                        p2[15] = i2[15] XOR c1[15]          // because i2[15] will be the same, is the right value
                        
        byte 14:
                        c1_magic[15] = i2[15] XOR 0x02      // we need the actual value that produces the pad
                        i2[14] = 0x02 XOR c1_magic[14]      // c1_magic[14] will be a guess in range(256) 
                        p2[14] = i2[14] XOR c1[14]          // because i2[14] will be the same, is the right value              
                        
        byte 13:
                        c1_magic[14] = i2[14] XOR 0x03      // we need the actual value that produces the pad
                        i2[13] = 0x03 XOR c1_magic[13]      // c1_magic[13] will be a guess in range(256) 
                        p2[13] = i2[13] XOR c1[13]          // because i2[13] will be the same, is the right value 
        
        byte 12:
                        c1_magic[13] = i2[13] XOR 0x04      // we need the actual value that produces the pad
                        i2[12] = 0x04 XOR c1_magic[12]      // c1_magic[12] will be a guess in range(256) 
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
            c1_magic[AES.block_size-j-1] = (i+1) ^ i2[AES.block_size-j-1]
        c1_magic = bytes(c1_magic)
        
        # Trying all possible values for a byte
        for guess in range(2**8):
            c1_magic = list(c1_magic)
            c1_magic[AES.block_size-i-1] = guess
            c1_magic = bytes(c1_magic)

            # Sending data
            conn = remote(HOST, PORT)
            try:
                conn.send(IV)
                conn.send(c1_magic + c2)
            except:
                print("Error")
            response = conn.recv()
            conn.close()
            
            # Checking response
            if response == b'OK':
                i2[AES.block_size-1-i] = guess ^ (i+1)
                p2[AES.block_size-1-i] = c1[AES.block_size-1-i] ^ i2[AES.block_size-1-i]
                print(bytes(p2))
                break
    
    return bytes(p2)


if __name__ == '__main__':
    
    # Retrieving number of blocks
    nblocks = get_n_block(ciphertext)
    p = []
    
    # Decrypting ciphertext[16:]
    for b in range(nblocks-1):
        c1 = get_nth_block(nblocks-2-b, ciphertext)
        c2 = get_nth_block(nblocks-1-b, ciphertext)
    
        p.append(guess_block(c1, c2))
        
    # Decrypting first block of ciphertext, based on IV
    p.append(guess_block(IV, get_nth_block(0, ciphertext)))
    
    # Printing result
    print('[!] Decryption completed: ', end='')
    print(b''.join(reversed(p)))
    