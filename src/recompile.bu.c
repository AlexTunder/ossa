#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char launch = 1;

int main(int argc, char **argv){
    char settings = 0;
    printf("[*]Updating osmic.exe \n");
    printf("[*]Updating...\n");
    printf("[*]Killing proccess...\n");
    system("powershell Stop-Process -Name \"osmic\" -Force");
    system("powershell Stop-Process -Name \"powershell\" -Force");
    printf("[*]Bulding config...\n");
    char str[1024];
    for(int i = 0; i < argc; i++){
        if(!strcmp(argv[i], "norun")) settings |= 0b01;
        else if(!strcmp(argv[i], "killMyself")) settings |= 0b10;
        else if(!strcmp(argv[i], "O3")) settings |= 0b100;
        else if(!strcmp(argv[i], "debug")) settings |= 0b1000;
    }
    sprintf(str, "make update");
    if(settings & 0b01) launch = 0;
    printf("  (%c) Run after build\n", settings & 0b01 ? '-':'+');
    printf("  (%c) Kill updater on start\n", settings & 0b10 ? '+':'-');
    printf("  (%c) Optimization options\n", settings & 0b100 ? '+':'-');
    printf("  (%c) Debug options\n", settings & 0b1000 ? '+':'-');
    printf("[*]Compiling...\n[*]");
    system(str);
    if(!launch)
        printf("[*]Done!\n");
    else
        printf("[*]Launching...\n");
    char *call = (char*)malloc(1024);
    strcpy(call, "");
    strcat(call, ".\\osmic.exe");
    if(settings & 0b10)
        {strcat(call, " killUpdater "); strncat(call, argv[0]+2, strlen(argv[0])-6);}
    system(call);
    // if(settings & 0b01)
    return 0;
}