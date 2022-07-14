import socket
import sys
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
from Crypto.Random.random import getrandbits

# DATA
HOST = '127.0.0.1'
PORT = 12345
ecb_oracle_key = b'\x1e\x86\x114\x0b\x8d6k`\xb1\xdc\xb5\xa9\xc7,\xe8A\xe2\x1c\x0bk\x93Lc\xc0\xa9\xce\xae\xcc.z\xd2'
ECB_MODE = 0
CBC_MODE = 1

# INIT SOCKET
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print('Socket created')

# BINDING SOCKET
try:
    s.bind((HOST, PORT))
except socket.error as msg:
    print('Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
    sys.exit()
print('Socket bind complete')

# LISTENING
s.listen(10)
print('Socket now listening')

# wait to accept a connection - blocking call
counter = 10
while counter:
    conn, addr = s.accept()
    print('A new encryption requested by ' + addr[0] + ':' + str(addr[1]))

    #select a mode of operation: ECB = 0, CBC = 1
    selected_mode = getrandbits(1)
    print("Selected mode = ",end='')
    if(selected_mode == ECB_MODE):
        print("ECB")
    else:
        print("CBC")

    # receive the chosen plaintext from the user
    input0 = conn.recv(1024).decode()
    message = "This is what I received: " + input0 + " -- END OF MESSAGE"
    print("Plaintext: " + message)

    # encrypt plaintext with chosen mode
    if(selected_mode == ECB_MODE):
        cipher = AES.new( ecb_oracle_key, AES.MODE_ECB )
    else:
        cipher = AES.new( ecb_oracle_key, AES.MODE_CBC )

    # send ciphertext
    message = pad(message.encode(),AES.block_size)
    ciphertext = cipher.encrypt(message)
    conn.send(ciphertext)

    counter -= 1

s.close()