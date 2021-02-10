import socket
import datetime

def to_integer(dt_time):
    return 10000*dt_time.year + 100*dt_time.month + dt_time.day

sock = socket.socket()
sock.bind(('127.0.0.1', 8000))
sock.listen(1024)
while True:
    conn, addr = sock.accept()
    print('connected: ', addr)
    while True:
        data = conn.recv(1024)
        # print('getted data: ', data)
        if not data:
            break
        words = data.split()
        print(words)
        if words[0] == b'OSSA-PTC:':
            conn.send(b"OSSA-PTC: 000f0010\n")
            print('New ossa ptc-c incoming')
    conn.close()
    print(addr, 'disconnected')