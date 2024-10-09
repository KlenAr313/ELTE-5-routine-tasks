import sys
import struct

types = ['f ? c', 'c 9s i', 'i ? f', 'c f 9s']
for i in range(0,4):
    with open(sys.argv[i+1], 'rb') as data:
        format = struct.Struct(types[i])
        values = data.read(format.size)
        print(format.unpack(values))

print(struct.pack('17s i ?', b'elso', 57, True))
print(struct.pack('f ? c', 60.5, False, b'X'))
print(struct.pack('i 15s f', 48, b'masodik', 67.9))
print(struct.pack('c i 18s', b'Z', 79, b'harmadik'))