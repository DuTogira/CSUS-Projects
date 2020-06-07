# Austin Smothers
# CSC 138
# socket2.py
# A simple mail client

import ssl
import base64
from socket import *
user = base64.b64encode('at.smothers@gmail.com')
key = base64.b64encode('wouldnt you like to know')

msg = "\r\n I love computer networks!"
endmsg = "\r\n.\r\n"
# Choose a mail server (e.g. Google mail server) and call it mailserver
mailserver = ('smtp.gmail.com', 587)
# Create socket called clientSocket and establish TCP connection with mailserver
# Fill in start
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect(mailserver)
# Fill in end

recv = clientSocket.recv(1024)
print recv
if recv[:3] != '220':
    print '220 reply not received from server.'

# Send HELO command and print server response.
heloCommand = 'HELO Alice\r\n'
clientSocket.send(heloCommand)
recv1 = clientSocket.recv(1024)
print(recv1)
if recv1[:3] != '250':
    print '250 reply not received from server.'

# Send STARTTLS so that we can use gmail servers
startCommand = 'STARTTLS\r\n'
clientSocket.send(startCommand)
recvStart = clientSocket.recv(1024)
print(recvStart)
if recvStart[:3] != '220':
    print '220 reply not received form server.'

sClientSocket = ssl.wrap_socket(clientSocket, ssl_version=ssl.PROTOCOL_SSLv23)
# Send AUTHLOGIN and authenticate for secure communication
authCommand = 'AUTH LOGIN '+user+'\r\n'
sClientSocket.send(authCommand)
recvAuth = sClientSocket.recv(1024)
print(recvAuth)
if recvAuth[:3] != '334':
    print '334 reply not received from server.'

keyCommand = key+'\r\n'
sClientSocket.send(keyCommand)
recvKey = sClientSocket.recv(1024)
print(recvKey)
if recvKey[:3] != '235':
    print '235 reply not received from server.'

# Send MAIL FROM command and print server resposne
# Fill in start
mailCommand = 'MAIL FROM:<at.smothers@gmail.com>\r\n'
sClientSocket.send(mailCommand)
recv2 = sClientSocket.recv(1024)
print(recv2)
if recv2[:3] != '250':
    print('250 reply not received from server.')
# Fill in end

# Send RCPT TO command and print server response
# Fill in start
rcptCommand = 'RCPT TO:<asmothers@csus.edu>\r\n'
sClientSocket.send(rcptCommand)
recv3 = sClientSocket.recv(1024)
print(recv3)
if recv3[:3] != '250':
    print('250 reply not received from server.')
# Fill in end

# Send DATA command and print server response
# Fill in start
dataCommand = 'DATA\r\n'
sClientSocket.send(dataCommand)
recv4 = sClientSocket.recv(1024)
print(recv4)
if recv4[:3] != '354':
    print('354 reply not received from server.')
# Fill in end

# Send message data
# Fill in start
send_to = 'To: asmothers@csus.edu\r\n'
send_from = 'From: at.smothers@gmail.com\r\n'
send_subj = 'Subject: Hello World!\r\n'
sClientSocket.send(send_to.encode())
sClientSocket.send(send_from.encode())
sClientSocket.send(send_subj.encode())
sClientSocket.send(msg.encode())
# Fill in end

# Message ends with a single period.
# Fill in start
sClientSocket.send(endmsg.encode())
recv5 = sClientSocket.recv(1024)
print(recv5)
if recv5[:3] != '250':
    print('250 reply not received from server.')
# Fill in end

# Send QUIT command and get server response
# Fill in start
quitCommand = 'QUIT\r\n'
sClientSocket.send(quitCommand)
recv6 = sClientSocket.recv(1024)
print(recv6)
if recv6[:3] != '221':
    print('221 reply not received from server.')
# Fill in end
