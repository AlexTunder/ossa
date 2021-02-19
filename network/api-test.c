#include "./api.h"
#include "../core/base.h"

#include <stdlib.h>
#include <stdio.h>

int returnCode = 0;

int main(){
    
    returnCode = setServer("127.0.0.1", 8000);
    printf("Connecting...\n");
    
    if(returnCode < 0){
        printf("Failed to connect to server (%i return code)!\n", returnCode);
        if(returnCode == OSSA_NET_INVALID_PROTO){
            printf("Bad protocol syntax\n");
        }
        closeServer(0);
        printf("Error code: %i\n", returnCode);
        return -1;
    }
    
    printf("Auth...\n");
    returnCode = authServer("alexthunder", "apature");
    
    if(returnCode == OSSA_AUTH_FAILED){
        printf("Auth failed\n");
        closeServer(0);
        printf("Error code: %i\n", returnCode);
        return -1;
    }
    
    struct Message *msg = (struct Message*)malloc(sizeof(struct Message));
    *msg = makeMes("Hello!", 21);
    printf("Sending message...\n");
    returnCode = sendMessage(msg);

    if(returnCode < 0) printf("Failed to send message: %i\n", returnCode);
    else printf("Count of messages: %i\n", returnCode);
    
    closeServer(0);
}