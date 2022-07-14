from webbrowser import get
from Crypto.Random import get_random_bytes
from Crypto.Cipher import AES
from base64 import b64decode, b64encode
import json

def Alice(key, IV):
    
    # Header and Payload
    header = b'This requires only authentication'
    payload = b'This requires both authentication and encryption'
    
    # Cipher 
    cipher = AES.new(key, AES.MODE_GCM, IV)
    
    # Computing data
    cipher.update(header)                                   # Requires authentication
    ciphertext, tag = cipher.encrypt_and_digest(payload)    # Requires authentication and encryption
    
    # Creating packet sent to Bob
    json_keys = ['nonce', 'header', 'ciphertext', 'tag']
    json_vals = [cipher.nonce, header, ciphertext, tag]
    json_vals_b64 = [ b64encode(x).decode() for x in json_vals ]
    data = json.dumps(dict(zip(json_keys, json_vals_b64)))
    
    return data

def Bob(key, IV, data: json):
    
    # Received
    print('Received data is: ')
    print(data)
    
    # Loading object
    data = json.loads(data)
    json_keys = ['nonce', 'header', 'ciphertext', 'tag']
    json_vals = {k: b64decode(data[k]) for k in json_keys}
    
    # Cipher object
    cipher = AES.new(key, AES.MODE_GCM, IV)
    
    # Computing
    cipher.update(json_vals['header'])
    try:
        payload = cipher.decrypt_and_verify(json_vals['ciphertext'], json_vals['tag'])
        print("Verification OK!")
        print("Payload is: " + payload.decode())
    except (ValueError, KeyError):
        print("Verification FAILED!")


def main():
    
    # Shared data
    # Key and IV
    key = get_random_bytes(AES.key_size[2])
    IV = get_random_bytes(AES.block_size)
    
    # Alice computes AEAD
    data = Alice(key, IV)
    
    # Bob receives data and computes
    result = Bob(key, IV, data)

if __name__ == '__main__':
    main()