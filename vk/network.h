
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

struct Syncer{
    WSADATA *wsaData; //DO NOT TOUCH!
    SOCKET ConnectSocket
};