#!/usr/bin/python
################################################################
#                        clientserv.py                         #
# Kierin Doherty (dohertki)                                    #
# CS372 Project 1 - chatserve.py                               #
# 30 Oct 2016                                                  #
#                                                              #
# Description: Program is a simple chat server. Program        #
#              utilizes TCP via system sockets. Server takes   #
#              turns exchanging messages with client.          #
#                                                              #
# Input: The program takes the following argument:             #
#              $  ./chatserve.py [port#]                       #
#                                                              #
#                                                              #
################################################################ 



import socket 
import sys 
import select 
import time 


HOST = '' #Symbolic name meaning all available interfaces
#PORT = 1850 
S = 'Server> '
if  len(sys.argv) == 2:
    PORT = int(sys.argv[1])
    print 'Port:' + str(PORT) 
else:
    print 'Missing argument'
    print 'Usage: ./clientserv.py [port#]'
    exit()

# Python code for the communicating with network
# socket is based on network programming tutorial found at:
# http://www.binarytides.com/python-socket-programming-tutorial/

try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error, msg:
    print 'Failed to create. socket. Error code: ' + str(msg[0])   
    sys.exit()
print 'Socket Created'

s.setblocking(0)


try:
	s.bind((HOST, PORT))
except socket.error, msg:
	print 'Bind failed. Error Code: ' + str(msg[0]) + ' Message ' + msg[1]
	sys.exit()



s.listen(10) #Parameter backlog...the number of incoming connection kept waiting

print 'Socket listening'


connections = list([])
connections.append(s)


################################################################ 
#                       readMessage(conn)                      #     
# Use: Function recieves messages to clients connected         #
#      to the server                                           #
# Input: conn: Pointer to a socket connection                  #
# Output: function prints messages to the screen               #
################################################################

def readMessage(conn):
    data = conn.recv(2056)
    print data
    
    if data == '\quit':
        print '....disconnecting user'
        conn.close()
        conn.send('\n')
        connections.remove(conn);
        print 'List'.join(map(str,connections))    
    else:
        print 'noquit'

################################################################
#                          sendMessage()                       #
# Use: Function gets message from the user and                 #
#     send it to the socket connection                         #
# Input: conn: Point to a socket connection                    #
# Output: none.                                                #
################################################################

def sendMessage(conn):
    data = raw_input("Server> ")
    conn.send(data)



while 1:
    print ','.join(map(str,connections))    
    try:
        ready_to_read, ready_to_write, in_error = select.select(connections ,[],[])
    except socket.error:
       break 

   # else:
    for sock in ready_to_read:
        
        if sock == s:
            conn, addr = s.accept()
            print 'Connected with Server ' + addr[0] + ' : ' + str(addr[1])
            connections.append(conn)
            readMessage(conn)
            conn.send('Chat server reply, ok!. Don''t eat green mushrooms') 
        else:     
            readMessage(conn)
            sendMessage(conn)        

    time.sleep(0)
    



conn.close()
s.close()


