#include "./api.h"

#include <stdio.h>

int returnCode = 0;

int main(){
    returnCode = setServer("127.0.0.1", 8000);
    if(returnCode < 0){
        printf("Failed to connect to server!\n");
    }
    closeServer(12);
}