#include "./api.h"

#include <stdio.h>

int returnCode = 0;

int main(){
    returnCode = setServer("127.0.0.1", 8000);
    if(returnCode < 0){
        printf("Failed to connect to server (%i return code)!\n", returnCode);
        if(returnCode == OSSA_NET_INVALID_PROTO){
            printf("Bad protocol syntax\n");
        }
    }
    printf("Auth...\n");
    returnCode = authServer("alexthunder", "apature");
    if(returnCode == OSSA_AUTH_FAILED){
        printf("Auth failed\n");
    }
    printf("Pushing user...\n");
    returnCode = useraddServer("alex", "hentai");
    if(returnCode < 0){
        printf("failed to add new user\n");
    }
    else printf("New user's UID: %i\n", returnCode);
    closeServer(0);
}