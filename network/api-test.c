#include "./api.h"

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
    printf("Pushing user...\n");
    returnCode = useraddServer("alex", "hentai");
    if(returnCode < 0){
        printf("failed to add new user\n");
        closeServer(0);
        printf("Error code: %i\n", returnCode);
        return -1;
    }
    else printf("New user's UID: %i\n", returnCode);
    closeServer(0);
}