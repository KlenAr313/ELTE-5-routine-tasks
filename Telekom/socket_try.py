import socket 

protocolname = 'tcp'
for i in range(1,101):
    try:
        print ("Port: %s => service name: %s" %(i, socket.getservbyport(i, protocolname)))
    except:
        print("No name")