import socket
import datetime

def to_integer(dt_time):
    return 10000*dt_time.year + 100*dt_time.month + dt_time.day

users = [b"alexthunder"]
hashs = [b'bf21913445697ef6a9211d7f15b3dd0a']
access = [b'ALL']

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
        if words[1] == b'000f0010':
            conn.send(b"OSSA-PTC: 000f0010\n")
            print("OSSA-INDENT")
        elif words[1] == b'000f001c':
            appendD = 1
            for i in range(0, len(users)):
                if (users[i] == words[3]):
                    appendD = 0
                    if hashs[i] == words[5]:
                        snd = b"OSSA-PTC: 000f0a1c\nUserid: "+ str(i).encode() +b'\n'
                        print("send:", snd)
                        conn.send(snd)
                        break
            if (appendD == 1):
                conn.send(b"OSSA-PTC: f00f0f1c\n")
    conn.close()
    print(addr, 'disconnected')