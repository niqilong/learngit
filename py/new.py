import socket

s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
s.bind(('127.0.0.1',9999))
while True:
    try:
        data1,addr1=s.recvfrom(1024)
        print(data1)
    except:
        print('except')
        continue
    break

