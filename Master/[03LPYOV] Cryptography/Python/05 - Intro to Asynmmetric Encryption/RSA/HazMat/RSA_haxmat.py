'''
Implementing RSA with HazMat library
'''

from hmac import digest
from cryptography.hazmat.primitives.hashes import SHA256, Hash
from cryptography.hazmat.primitives.asymmetric import rsa, padding, utils
from cryptography.hazmat.backends import default_backend


if __name__ == '__main__':
    # Generate KEYs
    KEY_pri = rsa.generate_private_key(
        public_exponent = 65537,
        key_size = 2048,
        backend = default_backend()
    )
    KEY_pub = KEY_pri.public_key()
    
    # ----------------------------------------------
    #           ENCRYPTION AND DECRYPTION
    # ----------------------------------------------
    
    # Encrypting message
    message = b'This is a super secure message'
    C = KEY_pub.encrypt(
        plaintext = message,
        padding = padding.OAEP(
            mgf = padding.MGF1(
                algorithm = SHA256()
            ),
            algorithm = SHA256(),
            label = None
        )
    )
    print('[!] Ciphertext is: ', C.hex())
    
    # Decrypting the message
    D = KEY_pri.decrypt(
        ciphertext = C,
        padding = padding.OAEP(
            mgf = padding.MGF1(
                algorithm = SHA256()
            ),
            algorithm = SHA256(),
            label = None
        )
    )
    print('[!] Decrypted message is: ', D.decode())
    

    # ----------------------------------------------
    #               DIGITAL SIGNATURE
    # ----------------------------------------------
    
    # Signing single message
    message = b'This is the message to sign!'
    
    signature = KEY_pri.sign(
        data = message,
        padding = padding.PSS(
            mgf = padding.MGF1(SHA256()),
            salt_length = padding.PSS.MAX_LENGTH
        ),
        algorithm = SHA256()
    )
    print('[!] This is the signature: ', signature.hex())    
    
    try:
        KEY_pub.verify(
            signature = signature,
            data = message,
            padding = padding.PSS(
                mgf = padding.MGF1(SHA256()),
                salt_length = padding.PSS.MAX_LENGTH
            ),
            algorithm = SHA256()
        )
        print('[!] Signature OK!')
    except: 
        print('[!] Signature failed!')
    
    # Signing two messages together
    messageA = b'This is a piec'
    messageB = b'e of the message'
    
    hasher_send = Hash(SHA256(), default_backend())
    hasher_send.update(messageA)
    hasher_send.update(messageB)
    digest = hasher_send.finalize()
    print('[!] This is the hash of the messages: ', digest.hex())
    
    signature = KEY_pri.sign(
        data = digest,
        padding = padding.PSS(
            mgf = padding.MGF1(SHA256()),
            salt_length = padding.PSS.MAX_LENGTH
        ),
        algorithm = utils.Prehashed(SHA256())
    )
    print('[!] This is the signature: ', signature.hex())
    
    
    # Receiver
    messageA = b'This is a piec'
    messageB = b'e of the message'
    hasher_recv = Hash(SHA256(), default_backend())
    hasher_recv.update(messageA)
    hasher_recv.update(messageB)
    digest_recv = hasher_recv.finalize()
    print('[!] This is the hash of the messages: ', digest_recv.hex())
    
    try:
        KEY_pub.verify(
            signature = signature, 
            data = digest_recv,
            padding = padding.PSS(
                mgf = padding.MGF1(SHA256()),
                salt_length = padding.PSS.MAX_LENGTH
            ),
            algorithm = utils.Prehashed(SHA256())
        )
        print('[!] Signature OK!')
    except: 
        print('[!] Signature failed!')
    
    
    
    