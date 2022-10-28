'''
Implementing RSA with pycryptodome library
------------------------------------------
'''

from Crypto.PublicKey import RSA
from Crypto.Hash import SHA256
from Crypto.Signature import PKCS1_PSS
from Crypto.Cipher import PKCS1_OAEP

# Some data
LENGTH = 2048

if __name__ == '__main__':
    
    # Generating RSA key pair
    RSA_KeyPair = RSA.generate(LENGTH)
    
    # Writing private key to file
    with open('private.pem', 'wb') as f_in:
        f_in.write(RSA_KeyPair.exportKey(
            format = 'PEM',
            passphrase = 'supersecurepassword',
            pkcs = 8
        ))
        f_in.close()
        
    # Writing public key to file
    with open('public.pem', 'wb') as f_in:
        f_in.write(RSA_KeyPair.public_key().exportKey(
            format = 'PEM',
            passphrase = 'supersecurepassword'
        ))
        f_in.close()
        
    
    # ----------------------------------------------
    #           ENCRYPTION AND DECRYPTION
    # ----------------------------------------------
    
    message = b'This a super secret message!'
    
    # Creating the cipher to encrypt the message (i.e. public key)
    cipher_pub = PKCS1_OAEP.new(RSA_KeyPair.public_key())
    C = cipher_pub.encrypt(message)
    print('[!] Encrypted message is: ', C.hex())
    
    # Creating the cipher to decrypt the ciphertext (i.e. private key)
    cipher_pri = PKCS1_OAEP.new(RSA_KeyPair)
    message = cipher_pri.decrypt(C)
    print('[!] Message is: ' + message.decode())
    
    
    # ----------------------------------------------
    #               DIGITAL SIGNATURE
    # ----------------------------------------------
    
    message = b'This an important message to sign!'
    
    # Creating sign obj
    hash = SHA256.new(message)
    signature = PKCS1_PSS.new(RSA_KeyPair).sign(hash)
    print('[!] Signature is: ', signature.hex())
    
    # Creating verify obj
    hash = SHA256.new(message)
    verifier = PKCS1_PSS.new(RSA_KeyPair.public_key())
    try:
        verifier.verify(hash, signature)
        print('[!] Signature OK!')
    except: 
        print('[!] Signature INVALID!')
    
    