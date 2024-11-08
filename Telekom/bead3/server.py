from socket import socket, AF_INET, SOCK_STREAM, timeout, SOL_SOCKET, SO_REUSEADDR
import sys
import struct
from select import select
import random

TCP_IP = sys.argv[1]
TCP_PORT = int(sys.argv[2])

server_addr = (TCP_IP, TCP_PORT)
packer = struct.Struct('c i')

theNumber = random.randint(0, 100)
found = False
print("TheNum: ", theNumber )

with socket(AF_INET, SOCK_STREAM) as server:
    server.bind(server_addr)
    server.settimeout(1.0)
    server.listen(1)
    server.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)

    sockets = [server]

    while True:
        r,w,e = select(sockets, [], [], 1)

        if not (r or w or e):
            continue

        for s in r:
            if s is server:
                #Client connenction
                client, client_addr = s.accept()
                sockets.append(client)

            else:
                question = s.recv(packer.size)
                if not question:
                    sockets.remove(s)
                    s.close()

                else:
                    unp_data = packer.unpack(question)
                    print(unp_data)
                    rel = unp_data[0].decode()
                    num = unp_data[1]
                    resp = b'V'
                    if not found:
                        if rel == '<':
                            if theNumber < num: resp = b'I'
                            else: resp = b'N'
                        elif rel == '>':
                            if theNumber > num: resp = b'I'
                            else: resp = b'N'
                        elif rel == '=':
                            if theNumber == num: 
                                resp = b'Y'
                                found = True
                            else: resp = b'K'
                    msg = (resp, 0)
                    #print("Sent ", msg)
                    s.sendall(packer.pack(*msg))


                 
