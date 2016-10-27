#!/usr/bin/python
# Simple client connects to google. 



#http://www.binarytides.com/python-socket-programming-tutorial/
import socket 
import sys # exit()
# Test.py

print "Hello World!"


HOST = '' #Symbolic name meaning all available interfaces
PORT = 8888



""" My block of text
Use bash command [$ which python] to find the path for the shebang line
for Unix.

"""
#try:	
	#Address Family: AF_/iNET (IPv4)
	#Type: SOCK_STREAM (TCP)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#except socket.error, msg:
#	print 'Failed to create. socket. Error code: ' + str(msg[0] + ', Error message: ' + msg[1] 
#	sys.exit();
print 'Socket Created'
"""This code will bind socket to a particular address and port. 

"""
try:
	s.bind((HOST, PORT))
except socket.error, msg:
	print 'Bind failed. Error Code: ' + str(msg[0]) + ' Message ' + msg[1]
	sys.exit()

""" This code will listen for connections




"""

s.listen(10) #Parameter backlog...the number of incoming connection kept waiting

print 'Socket listening'

"""Wait to accept a connection- blocking call
"""




#now keep talking to the client
while 1:

    conn, addr = s.accept()
    print 'Connected with ' + addr[0] + ' : ' + str(addr[1])
    
    data = conn.recv(1024)
    reply = 'OK... ' + data
    if not data:
        break
        
    conn.sendall(data)

conn.close()
s.close()

