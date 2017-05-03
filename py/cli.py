import socket,struct

s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
t=struct.pack('<H',int(188)%0xFFFF)
data="\x01\x02"+t+"\x09"+"\x00"*15
s.sendto(data,('127.0.0.1',9999))
