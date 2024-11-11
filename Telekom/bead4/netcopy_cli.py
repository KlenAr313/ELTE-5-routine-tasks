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

hash_data = hashlib.md5()

with socket(AF_INET, SOCK_STREAM) as client:
    client.connect(server_addr)
    
    with open(FILE_PATH, 'br') as file:
        data = file.read(256)
        while data != b'':
            client.send(data)
            hash_data.update(data)
            data = file.read(256)
        client.send(b'')

with socket(AF_INET, SOCK_STREAM) as client_for_chsum:
    client_for_chsum.connect(chsum_server_addr)
    client_for_chsum.send(bytes('|'.join(['BE', str(FILE_ID), str(60), str(hash_data.digest_size), str(hash_data.digest())]), encoding='utf-8'))
    print(client_for_chsum.recv(2).decode('utf-8'))