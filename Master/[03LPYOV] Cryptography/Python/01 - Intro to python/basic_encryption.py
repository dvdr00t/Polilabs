from Crypto.Random import get_random_bytes
from Crypto.Cipher import ChaCha20, AES
from Crypto.Util.Padding import pad, unpad
from base64 import *

def basic_STREAM():
    
    print('------------------------------------------------')
    print('             BASIC STREAM CIPHER                ')
    print('------------------------------------------------')
    
    '''
    Implement a basic version of ChaCha20 encryption
    ------------------------------------------------
    The ChaCha20 is a stream cipher algorithm built on a pseudorandom function based on add-rotate-XOR (ARX) 
    operations: 32-bit addition, bitwise addition (XOR) and rotation operations.
    
        - Key sizes:    128 or 256 bits
        - State size	512 bits
        - Rounds:       20
    '''
    
    # Original message
    plaintext = b'This is a secret message'
    plaintext_long = b'This is a very long secret message that you don\'t actually need to read...'
    print('plaintext length is ' + str(len(plaintext)))
    print('plaintext_long length is ' + str(len(plaintext_long)))
    
    # Key and nonce
    key = get_random_bytes(ChaCha20.key_size)   # ChaCha20.key_size = 32
    nonce = get_random_bytes(12)
    
    # Creating the cipher object
    cipher_enc = ChaCha20.new(key=key, nonce=nonce)
    cipher_dec = ChaCha20.new(key=key, nonce=nonce)
    
    # Encryption
    ciphertext = cipher_enc.encrypt(plaintext)
    ciphertext_long = cipher_enc.encrypt(plaintext_long)
    print('Ciphertext is: ' + b64encode(ciphertext).decode())
    print('Ciphertext_long is: ' + b64encode(ciphertext_long).decode())
    print('Ciphertext_long length is ' + str(len(ciphertext_long)))
    
    # Encryption
    decrypted = cipher_dec.decrypt(ciphertext)
    decrypted_long = cipher_dec.decrypt(ciphertext_long)
    print('Plaintext is: ' + decrypted.decode())
    print('Plaintext_long is: ' + decrypted_long.decode())
    
    

def basic_BLOCK():
    
    print('------------------------------------------------')
    print('              BASIC BLOCK CIPHER                ')
    print('------------------------------------------------')
    
    '''
    Implement a basic version of AES encryption
    -------------------------------------------
    The Advanced Encryption Standard (AES), also known by its original name Rijndael, is a specification for 
    the encryption of electronic data.
    The algorithm described by AES is a symmetric-key block-cipher algorithm, meaning the same key is used for both 
    encrypting and decrypting the data. 
    
        - Key sizes:    128, 192 or 256 bits
        - Block sizes:  128 bits
        - Rounds:       10, 12 or 14 (depending on key size)
    '''
    
    # Original messages
    plaintext_aligned = b'This is a message 32-bytes long!'
    plaintext_unaligned = b'This is a very long secret message that you don\'t actually need to read...'
    print('plaintext_aligned length is ' + str(len(plaintext_aligned)))
    print('plaintext_unaligned length is ' + str(len(plaintext_unaligned)))

    # Key and IV 
    key = get_random_bytes(AES.key_size[2])     # AES.key_size = [16, 24, 32]
    IV = get_random_bytes(AES.block_size)       # AES.block_size = 16
    
    # Creating the cipher object
    cipher_enc = AES.new(key, AES.MODE_CBC, IV)
    cipher_dec = AES.new(key, AES.MODE_CBC, IV)
    
    # Encryption of plaintext aligned
    ciphertext_aligned = cipher_enc.encrypt(plaintext_aligned)
    print('ciphertext_aligned is: ' + b64encode(ciphertext_aligned).decode())
    
    # Encryption of plaintext unaligned
    ciphertext_unaligned = cipher_enc.encrypt(pad(plaintext_unaligned, AES.block_size))
    print('ciphertext_unaligned length is ' + str(len(ciphertext_unaligned)))
    print('ciphertext_unaligned is: ' + b64encode(ciphertext_unaligned).decode())
    
    # Decryption of plaintext aligned
    plaintext_aligned_decrypted = cipher_dec.decrypt(ciphertext_aligned)
    print('Plaintext is: ' + plaintext_aligned_decrypted.decode())
    
    # Decryption of plaintext unaligned
    plaintext_unaligned_decrypted = cipher_dec.decrypt(ciphertext_unaligned)
    print('Plaintext is: ' + unpad(plaintext_unaligned_decrypted, AES.block_size).decode())

if __name__ == "__main__":
    basic_BLOCK()
    basic_STREAM()
    