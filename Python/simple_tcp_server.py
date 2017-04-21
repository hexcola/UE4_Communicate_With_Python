# python 2.7

import socket

HOST = "127.0.0.1"
PORT = 8891

s = socket.socket()

try:
    s.bind((HOST, PORT))

    try:
        s.listen(5)
        print "Server listening on port: %d" % PORT
        conn, addr = s.accept()


        while True:
            if conn:  print conn
            data = conn.recv(1024)
            if not data: break
            conn.send(data)
    except:
        print "Can not listen..."

except:
    print "Can not bind to %s:%d" % (HOST, PORT)
finally:
    s.close()
