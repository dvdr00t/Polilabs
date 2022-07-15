from pwn import *
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
from server_genCookie import encode_profile, profile_for

# Configuration data
HOST = '127.0.0.1'
PORT_GEN = 12345
PORT_TEST = 12445

if __name__ == '__main__':
    
    # Opening connection to the server that generates a cookie
    conn_gen = remote(HOST, PORT_GEN)
    email = b'aaaaaaa@b.com'        
    
    # Sending data
    conn_gen.send(email)
    cookie = conn_gen.recv()
    print("Encrypted cookie is: " + cookie.hex())
    
    # Computing cookie info
    # length of email is 13 bytes in order to align the start of the "admin" exactly at third block 
    cookie_info = encode_profile(profile_for(email.decode()))
    print(cookie_info[0:AES.block_size])
    print(cookie_info[AES.block_size:2*AES.block_size])
    
    # Computing cookie info
    # this time we try to align the "admin" word to the second block 
    padded_message = b'A'*10 + pad(b'admin', AES.block_size)
    cookie_info = encode_profile(profile_for(padded_message.decode()))
    print(cookie_info[0:AES.block_size])
    print(cookie_info[AES.block_size:2*AES.block_size])
    
    conn_gen.close()
    
    # Opening again connection to the server that generates a cookie
    conn_gen = remote(HOST, PORT_GEN)
    
    # Sending data
    conn_gen.send(padded_message)
    cookie_2 = conn_gen.recv()
    print()
    print()
    print(cookie_2)
    conn_gen.close()
    
    # Crafting authentication cookie and check on server
    auth_cookie = cookie[0:32] + cookie_2[16:32]
    conn_test = remote(HOST, PORT_TEST)
    conn_test.send(auth_cookie)
    response = conn_test.recv()
    
    print(response.decode())
    