import socket
import datetime

def to_integer(dt_time):
    return 10000*dt_time.year + 100*dt_time.month + dt_time.day

addr_active = []
addr_unames = []

users = [b"alexthunder"]
hashs = [b'bf21913445697ef6a9211d7f15b3dd0a']
access = [b'ALL']

sock = socket.socket()
sock.bind(('127.0.0.1', 8000))
sock.listen(1024)
while True:
    conn, addr = sock.accept()
    print('connected: ', addr)
    addr_active.append(addr)
    addr_unames.append("$none")
    while True:
        data = conn.recv(1024)
        # print('getted data: ', data)
        if not data:
            break
        words = data.split()
        print(words)
        if words[1] == b'000f0010':
            conn.send(b"OSSA-PTC: 000f0010\n\0")
            # print("OSSA-INDENT")
        elif words[1] == b'000f001c':
            appendD = 1
            for i in range(0, len(users)):
                if (users[i] == words[3]):
                    appendD = 0
                    if hashs[i] == words[5]:
                        snd = b"OSSA-PTC: 000f0a1c\nUserid: "+ str(i).encode() +b'\0'
                        print("send:", snd)
                        conn.send(snd)
                        #now, bind addr_active with addr_unames
                        for j in range(0, len(addr_active)):
                            if(addr_active[j] == addr):
                                addr_unames[j] = users[i]
                                break
                        break
                    else:
                        conn.send(b'OSSA-PTC: 000f0a0c\0')
            if (appendD == 1):
                conn.send(b"OSSA-PTC: f00f0f1c\0")
        elif words[1] == b'5c0f001c':
            #reg new user
            reg = 0
            for i in range(0, len(addr_active)-1):
                if addr_active[i] == addr:
                    for j in range(0, len(users)-1):
                        if(users[j] == addr_unames[i]):
                            if(access[j] == b'ALL'):
                                #append user to userlist
                                index = users.append(words[3])
                                hashs.append(words[6])
                                access.append(b'ALL')
                                snd = b"OSSA-PTC: 000f0b1c\nUID: "+ str(index).encode() +b'\nERROR: 0\0'
                                conn.send(snd)
                                reg = 1
                                break
                    break
            if(reg == 0):
                conn.send(b'OSSA-PTC: faafff1c\nERROR: 5\0')
        elif words[1] == b'4c0f001c':
            for i in range(0, len(addr_active)):
                if(addr_active[i] == addr):
                        if(addr_unames[i] != '$none'):
                            conn.send(b'OSSA-PTC: 0000f11a\nTotal: 15\n')
                            print("MO")
                        else:
                            conn.send(b'OSSA-PTC: 0000f10a\nERROR: 51\n')
                            print("MF")
        elif words[1] == b'000bf11a':
            for i in range(0, len(addr_active)):
                if(addr_active[i] == addr):
                        if(addr_unames[i] != '$none'):
                            conn.send(b'OSSA-PTC: 000bf11f\nCount: 1\nContent:next\n{\"body\":\"Hi!\",\"date\":\"16666213\",\"sender\":\"1\"}\n')
                            print("MU")
                        else:
                            conn.send(b'OSSA-PTC: 0000f10a\nERROR: 51\n')
                            print("MF")
        elif words[1] == b'000ba110':
            for i in range(0, len(addr_active)):
                if(addr_active[i] == addr):
                        if(addr_unames[i] != '$none'):
                            conn.send(b'OSSA-PTC: 002ba110\nCount: 3\nContent:next\n{\"username\":\"system\"}\nContent:next\n{\"username\":\"16666213\"}\nContent:next\n{\"username\":\"1\"}\n')
                            print("MU")
                        else:
                            conn.send(b'OSSA-PTC: 0000f10a\nERROR: 51\n')
                            print("MF")
        else:
            print("UnSy")
    conn.close()
    print(addr, 'disconnected')