from socket import socket, AF_INET, SOCK_STREAM, timeout, SOL_SOCKET, SO_REUSEADDR
import sys
from select import select
from time import time
from math import floor

TCP_IP = sys.argv[1]
TCP_PORT = int(sys.argv[2])

server_addr = (TCP_IP, TCP_PORT)

with socket(AF_INET, SOCK_STREAM) as server:
    server.bind(server_addr)
    server.listen()
    server.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    server.settimeout(1.0)

    sockets = [server]
    validations = {}
    cur_time = last_time = time()

    while True:
        readables, writeables, ex = select(sockets, [], [], 1)

        if not (readables or writeables or ex):
            continue

        for s in readables:
            if s is server:
                #Client connenction
                client, client_addr = s.accept()
                sockets.append(client)

            else:
                last_time = cur_time
                cur_time = time()

                for key in list(validations.keys()):
                    #validations[key][1] = int(validations[key][1]) - floor(cur_time - last_time)
                    if cur_time - last_time > validations[key][1]:
                        del validations[key]
                try:
                    data = s.recv(256)
                    if data:
                        msg = data.decode().split('|')
                        print(data.decode())
                        if msg[0] == 'BE':
                            validations[msg[1]] = (msg[1], int(msg[2]), int(msg[3]), msg[4]) #fileId, time, size, bytes
                            s.send(b'OK')
                        elif msg[0] == 'KI':
                            if msg[1] in validations.keys():
                                print('|'.join([str(validations[msg[1]][2]), str(validations[msg[1]][3])]))
                                s.send(bytes('|'.join([str(validations[msg[1]][2]), str(validations[msg[1]][3])]), encoding='utf-8'))
                            else: s.send(b'|0')
                except:
                    print()

        for e in ex:
            sockets.remove(e)
            e.close()

                