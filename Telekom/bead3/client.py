from socket import socket, AF_INET, SOCK_STREAM, timeout, SOL_SOCKET, SO_REUSEADDR
import sys
import struct
from math import floor

TCP_IP = sys.argv[1]
TCP_PORT = int(sys.argv[2])

server_addr = (TCP_IP, TCP_PORT)
packer = struct.Struct('c i')
minimum = 0
maximum = 100

with socket(AF_INET, SOCK_STREAM) as client:
    client.connect(server_addr)
    client.settimeout(1.0)

    guess = floor((minimum + maximum) / 2)
    client.sendall(packer.pack(b'>', int(guess)))

    while True:
        resp = client.recv(packer.size)
        resp = packer.unpack(resp)[0].decode()
        if resp == 'K' or resp == 'V' or resp == 'Y':
            break 
        else:
            if resp == 'I':
                minimum = guess + 1
            elif resp == 'N':
                maximum = guess

            if maximum != minimum:
                guess = floor((minimum + maximum) / 2)
                client.sendall(packer.pack(b'>', int(guess)))
            else:
                client.sendall(packer.pack(b'=', int(maximum)))
        