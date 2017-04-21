# python 2.7

"""
This is a simple python tcp client echo program

"""
import socket
import sys

HOST = "127.0.0.1"
PORT = 8891

s = socket.socket()

try:
    s.connect((HOST, PORT))
    while True:
        msg = raw_input('> ')
        if msg == "c":
            sys.exit()
        else:
            s.send(msg)
            print s.recv(1024)
except:
    print "Can Not Connect to %s:%d" % (HOST, PORT)
finally:
    s.close()
