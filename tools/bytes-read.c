#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
    if(argc < 2){
        printf("use %s [file] [bytes in line] {[h/b]}", argv[0]);
    }
    char sop = 0;
    char tp = 'h';
    if(argc >= 4){
        tp = *argv[3];
    }
    sop = (argc >= 5);
    FILE *target = fopen(argv[1], "r");
    unsigned char buf;
    int bil = atoi(argv[2]);
    int counter = 0;
    do{
        if(fread(&buf, 1, 1, target) == 0) break;
        if(counter == bil)
            {printf("\n");counter = 1;}
        else {counter++;
        printf(" ");}
        if(tp == 'h'){
            if(sop == 0)
                printf("%x", buf);
            else
                if(buf != '\n' || buf != '\r')
                    printf("%x(%c)", buf, buf);
                else
                    printf("%x(NL/RC)", buf);
        }
        else if(tp == 'b'){
            for(char i = 8; i >= 0; i--)
                printf("%c", buf & 1 << i ? '1' : '0');
            printf(" (%c)", buf);
        }
    }while(1);
    fclose(target);
    return 0;
}