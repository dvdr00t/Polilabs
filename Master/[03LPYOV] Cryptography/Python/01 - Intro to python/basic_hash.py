from email import message
from Crypto.Hash import SHA256, HMAC

def simple_hash():
    
    print('------------------------------------------------')
    print('                 BASIC HASH                     ')
    print('------------------------------------------------')
    
    
    # Hash Generator object
    hasher = SHA256.new()
    
    # Message to be hashed
    message = b'Compute the hash of this message'
    
    # Hashing
    hasher.update(message)
    hasher.update(b'Ops... you forgot this part of the text')
    
    # Result
    print('Hash is: ' + str(hasher.hexdigest()))
    print('Hash length is: ' + str(len(hasher.digest())))
    

def file_hash():
    
    print('------------------------------------------------')
    print('                 FILE HASH                      ')
    print('------------------------------------------------')
    
    # Hash Generator object
    hasher = SHA256.new()
    
    # Opening file
    with open(__file__, "rb") as f_input:
        hasher.update(f_input.read())
    
    # Result
    print('Hash is: ' + str(hasher.hexdigest()))
    print('Hash length is: ' + str(len(hasher.digest())))
    
def simple_MAC():
    
    print('------------------------------------------------')
    print('                 SIMPLE MAC                     ')
    print('------------------------------------------------')
    
    # Message
    message = b'This messaged will be MACed'
    
    # Key
    key = b'secretkey'
    
    # Hash Generator object
    hasher = HMAC.new(key, digestmod=SHA256)
    
    # MACing
    hasher.update(message)
    
    # Result
    print('MAC is: ' + str(hasher.hexdigest()))
    print('MAC length is: ' + str(len(hasher.digest())))
    
    # Verification
    verifier = HMAC.new(key, digestmod=SHA256)
    verifier.update(message + b'!')
    try:
        verifier.verify(hasher.digest())
        print("Yes! HMAC is the same!")
    except:
        print("Nope... Some error occurred")
        

def main():
    simple_hash()
    print()
    file_hash()
    print()
    simple_MAC()

if __name__ == '__main__':
    main()