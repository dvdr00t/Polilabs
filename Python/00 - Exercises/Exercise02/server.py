import socket
import sys
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad

key = b'0123456789abcdef0123456789abcdef'
HOST = 'localhost'
PORT = 12345


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print('Socket created')

try:
    s.bind((HOST, PORT))
except socket.error as msg:
    print('Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
    sys.exit()
print('Socket bind complete')

s.listen(10)
print('Socket now listening')

#wait to accept a connection - blocking call
while 1:
    conn, addr = s.accept()
    print('A new padding test requested by ' + addr[0] + ':' + str(addr[1]))

    # get the IV from the client
    iv = conn.recv(AES.block_size)
    # get the ciphertect from the client
    ciphertext = conn.recv(1024)

    #decrypts the ciphertext
    cipher = AES.new(key, AES.MODE_CBC, iv )

    try:
        unpad(cipher.decrypt(ciphertext),AES.block_size,style='x923')


    except ValueError:
        conn.send(b'0')
        continue


    conn.send(b'1')

    conn.close()

s.close()
