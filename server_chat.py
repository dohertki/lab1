#!/usr/bin/python
# Simple server. 



#http://www.binarytides.com/python-socket-programming-tutorial/
import socket 
import sys # exit()
import select # select()
# Test.py


HOST = '' #Symbolic name meaning all available interfaces
PORT = 1850 
SOCKET_LIST = [] #FIXME needs new name
RECV_BUFFER = 1024 #FIXME


""" My block of text
Use bash command [$ which python] to find the path for the shebang line
for Unix.

"""
try:	
	#Address Family: AF_/iNET (IPv4)
	#Type: SOCK_STREAM (TCP)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error, msg:
    print 'Failed to create. socket. Error code: ' + str(msg[0])   
    sys.exit()
print 'Socket Created'

#s.setblocking(0)

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
#set up lists for select()
#Sockets to be read



#now keep talking to the client
connections = list([])
connections.append(s)

S = 'Server> '


conn, addr = s.accept()

connections.append(conn)

while 1:
    print ' looping'    
    try:
        ready_to_read, ready_to_write, in_error = select.select(connections ,[],[])
    except socket.error:
        continue

   # else:
    for sock in ready_to_read:
        
        if sock == s:
            conn, addr = s.accept()
            print 'Connected with ' + addr[0] + ' : ' + str(addr[1])
            connections.append(conn)
            #conn.send('Chat server reply, ok!') 
        else:      
            data = conn.recv(2056)
            print data
#    for i in ready_to_wrire
#        else:
#            #local message
#            message = raw_input(S) 
#            conn.send('Server> ')

            conn.send(data)

    for sock in ready_to_write:
        print 'read?'
        conn.sendall('Server>' + data)



#>>> temp = data
#>>> temp
#>>> temp.find(">")



conn.close()
s.close()

