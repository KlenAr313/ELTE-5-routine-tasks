from socket import socket, AF_INET, SOCK_STREAM, timeout, SOL_SOCKET, SO_REUSEADDR
import sys
import hashlib

SRV_IP = sys.argv[1]
SRV_PORT = int(sys.argv[2])
CHSUM_SRV_IP = sys.argv[3]
CHSUM_SRV_PORT = int(sys.argv[4])
FILE_ID = int(sys.argv[5])
FILE_PATH = sys.argv[6]

server_addr = (SRV_IP, SRV_PORT)
chsum_server_addr = (CHSUM_SRV_IP, CHSUM_SRV_PORT)

with socket(AF_INET, SOCK_STREAM) as server:
    server.bind(server_addr)
    server.listen()

    conn, addr = server.accept()
    hash_data = hashlib.md5()
    with open(FILE_PATH, 'bw') as file:
        data = conn.recv(256)
        while data != b'':
            file.write(data)
            hash_data.update(data)
            data = conn.recv(256)
    
with socket(AF_INET, SOCK_STREAM) as client_for_chsum:
    client_for_chsum.connect(chsum_server_addr)
    client_for_chsum.send(bytes('|'.join(['KI', str(FILE_ID)]), encoding='utf-8'))

    resp = client_for_chsum.recv(256).decode().split('|')
    if resp[0] == 0 or str(resp[1]) != str(hash_data.digest()):
        print('CSUM CORRUPTED')
    else:
        print('CSUM OK')