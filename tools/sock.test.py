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
            conn.send(b"OSSA-PTC: 000f0010\n")
            print("OSSA-INDENT")
        elif words[1] == b'000f001c':
            appendD = 1
            for i in range(0, len(users)):
                if (users[i] == words[3]):
                    appendD = 0
                    if hashs[i] == words[5]:
                        snd = b"OSSA-PTC: 000f0a1c\rUserid: "+ str(i).encode() +b'\n'
                        print("send:", snd)
                        conn.send(snd)
                        #now, bind addr_active with addr_unames
                        for j in range(0, len(addr_unames)):
                            if(addr_active[j] == addr):
                                addr_unames[j] = users[j]
                                # print (j)
                                break
                        break
            if (appendD == 1):
                conn.send(b"OSSA-PTC: f00f0f1c\n")
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
                                snd = b"OSSA-PTC: 000f0b1c\rUID: "+ str(index).encode() +b'\rERROR: 0\n'
                                conn.send(snd)
                                reg = 1
                                break
                    break
            if(reg == 0):
                conn.send(b'OSSA-PTC: faafff1c\nERROR: 5\r')
                
    conn.close()
    print(addr, 'disconnected')