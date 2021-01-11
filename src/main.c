#define AIRLIB_PATH "/"
#define ENABLE_ACCESS   
// #include "..\airlib\OpenCppNet\Socket.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <process.h>
#include "./chat.h"

//CLI codes
#define CLI_OK_NOUT 0x00f0
#define CLI_OK_DROP 0x00f1
#define CLI_EXIT    0x06fc
#define CLI_ERRF    0x0600
#define CLI_CNF     0x06ff
#define CLI_BOS     0x06ac
#define CLI_NOPERM  0x0700
//Access codes
#define acc_read 1<<0  /* read messages */
#define acc_write 1<<1 /* send messages */
#define acc_users 1<<2 /* manage users */
#define acc_roler 1<<3 /* manage rolies */
#define acc_taggi 1<<4 /* tag anyone */
#define acc_evlog 1<<5 /* login as everyone */
#define h 0x1f

//Global variables
int last = 0;

int handleCommand(char comm[32][16], struct Chat *chat, int *me){
    if(!strcmp(comm[0], "exit") || !strcmp(comm[0], "q"))
        return CLI_EXIT;
    else if(!strcmp(comm[0], "drop")){
        // dropChatToFile(chat);
        char *fns[3] = {0x0,0x0,0x0};
        for(int i = 0; i < 32 && strcmp(comm[i], ""); i++){
            if(!strcmp(comm[i], "-chat")){
                fns[0] = comm[++i];
            } else if(!strcmp(comm[i], "-users")){
                fns[1] = comm[++i];
            } else if(!strcmp(comm[i], "-roles")){
                fns[2] = comm[++i];
            }
        }
        dropChatToFile(chat, fns[0], fns[1], fns[2]);
        return CLI_OK_DROP;
    } else if(!strcmp(comm[0], "useradd")){
        if(getUserAccess(*me, &chat->userList) & acc_users){
            if(comm[1] != NULL)
                if(strcmp(comm[1], "\0"))
                    return pushUser(comm[1], chat);
        }
        else printf("You haven't permission to this command. You need \'acc_users\' permission flag.\n");
        return CLI_OK_NOUT;
    } else if(!strcmp(comm[0], "lsuser")){
        int max = getUsersCount(chat);
        printf("Total: %i\nID\tName\n", max);
        for(int i = 0; i < max ; i++){
            printf("%i\t%s\n", i, getUsernameByID(i, &chat->userList));
        }
        return CLI_OK_NOUT;
    } else if(!strcmp(comm[0], "set")){
        if(!strcmp(comm[1], "me")){
            if(getUserAccess(*me, &chat->userList) & acc_evlog){
                if(strcmp(comm[2], "\0")){
                    *me = atoi(comm[2]);
                    return CLI_OK_NOUT;
                }
            }else{
                printf("No permission. Get \'acc_evlog\' flag for login as everyone of users or use \':login [userid] [password]\'\n");
                return CLI_NOPERM;
            }
        }else if(!strcmp(comm[1], "user")){
            if(getUserAccess(*me, &chat->userList) & acc_users){
                int target = atoi(comm[2]);
                if(!strcmp(comm[3], "role")){
                    if(getUserAccess(*me, &chat->userList) & acc_roler)
                        addUserToRole(&chat->roler, target, atoi(comm[4]), &chat->userList);
                    else{
                        printf("You should to have \'acc_roler\' permission flag do do that\n");
                    }
                } else if(!strcmp(comm[3], "ban")){
                    if(getUserAccess(*me, &chat->userList) & acc_roler)
                        addUserToRole(&chat->roler, target, 0, &chat->userList);
                    else{
                        printf("You should to have \'acc_roler\' permission flag do do that\n");
                    }
                } else if(!strcmp(comm[3], "name")){
                    struct UserList *counter = &chat->userList;
                    int i = 0;
                    do{
                    counter = counter->next; 
                    }while(counter->next != NULL && i < target);
                    free(counter->name);
                    counter->name = (char*)malloc(strlen(comm[4]));
                    strcpy(counter->name, comm[4]);
                }
            }else{
                printf("You should to have \'acc_users\' permission flag do do that\n");
            }
        }else if(!strcmp(comm[1], "role")){
            if(getUserAccess(*me, &chat->userList) & acc_roler){
                int target = atoi(comm[2]);
                if(!strcmp(comm[3], "access")){
                    int access = 1 << atoi(comm[4]);
                    getRoleByIndex(&chat->roler, target)->access |= access;
                    for(struct UsersLinks *ul = &getRoleByIndex(&chat->roler, target)->usersList; ul->next != NULL; ul = ul->next){
                        getUserById(&chat->userList, ul->id)->access |= access;
                    }
                }
            }else{
                printf("You should to have \'acc_roler\' permission flag do do that\n");
            }
        }
        else {printf("bad \'set\' parameter (%s)\n", comm[1]);return CLI_CNF;}
    } else if(!strcmp(comm[0], "view")){
        if(!strcmp(comm[1], "all")){
            last = 0;
        }else if(!strcmp(comm[1], "tail")){
            last = getMessagesCount(chat) - atoi(comm[2]);
        }
    } else if(!strcmp(comm[0], "lsrole")){
        printf("ID\tMemory\t\t\tRole name\tAccess code\tAccess number\n");
        if (strcmp(comm[1], "")){
            int index = atoi(comm[2]);
            printf("%i\t%p\t%s\t%x\t%i\n", index, getRoleByIndex(&chat->roler, index), getRoleByIndex(&chat->roler, index)->name, getRoleByIndex(&chat->roler, index)->access, getRoleByIndex(&chat->roler, index)->access);
        }else{
            for(int index = 0; index < getRolesCount(&chat->roler); index++){
                printf("%i\t%p\t%s\t%x\t%i\n", index, getRoleByIndex(&chat->roler, index), getRoleByIndex(&chat->roler, index)->name, getRoleByIndex(&chat->roler, index)->access, getRoleByIndex(&chat->roler, index)->access); 
            }
        }
    }else if(!strcmp(comm[0], "roleadd")){
        if(getUserAccess(*me, &chat->userList) & acc_roler)
            addRole(&chat->roler, comm[1], 0);
        else{
            printf("No You should to have \'acc_roler\' permission flag do do that\n");
        }
    } else if(!strcmp(comm[0], "recompile")){
        char settings = 0; //1 bit = basic, 2 bit = updater, 3 bit = norun, 4 - main only

        settings = 0b001;
        for(int i = 1; i < 4; i++){
            if(!strcmp(comm[i], "-all"))
                settings |= 0b011;
            else if(!strcmp(comm[i], "-updater"))
                {settings |= 0b010; settings &= ~0b001;}
            else if(!strcmp(comm[i], "-norun"))
                settings |=0b100;
            else if(!strcmp(comm[i], "-ma"))
                settings |= 0b1000;
            else if(!strcmp(comm[i], "-optimal"))
                settings |= 0b10000;
            else if(!strcmp(comm[i], "-debug"))
                settings |= 0b100000;
        }
        printf("Compiling rules: %i%i%i\n", (settings & 0b100) ? 1:0, (settings & 0b010) ? 1:0, (settings & 0b001) ? 1:0);
        if(settings & 0b010){
            printf("[*]Updating dist.exe\n[*]Killing proccess...\n");
            system("powershell Stop-Process -Name \"dist\" -Force");
            printf("[*]Compile....\n[*]");
            system("make updater");
            printf("[*]Enter bootstrap...\n");
        }if(settings & 0b001){
            char *updaterCom = (char*)malloc(1024);
            sprintf(updaterCom, ".\\dist.exe");
            if(settings & 0b0100)
                strcat(updaterCom, " norun");
            if(settings & 0b1000)
                strcat(updaterCom, " killMyself");
            if(settings & 0b10000)
                strcat(updaterCom, " O3");
            if(settings & 0b100000)
                strcat(updaterCom, " debug");
            region{
                char *sub = (char*)malloc(strlen(updaterCom)+32);
                strcpy(sub, updaterCom);
                sprintf(updaterCom, "%s -pid=%i", sub, _getpid());
                free(sub);
            }
            system(updaterCom);
            free(updaterCom);
        }
        else if( settings & 0b001) system(".\\dist.exe");
    } else if(!strcmp(comm[0], "inrole")){
        int user_id = atoi(comm[1]);
        int role_id = atoi(comm[2]);
        if(getRoleByIndex(&chat->roler, role_id) != NULL){
            printf("(%i)%s user in role (%i)%s: ", user_id, getUsernameByID(user_id, &chat->userList), role_id, getRoleByIndex(&chat->roler, role_id)->name);
            if(checkInRole(getRoleByIndex(&chat->roler, role_id), user_id))
                printf("YES\n");
            else printf("NO\n");
        }
    } else if(!strcmp(comm[0], "ban")){
        int user_id = atoi(comm[1]);
        addUserToRole(&chat->roler, user_id, 0, &chat->userList);
    } else if(!strcmp(comm[0], "long")){
        char *input = (char*) malloc(4294967295);
        char stop = *comm[1];
        char c = 0;
        unsigned long counter = 0;
        do{
            c = getchar();
            if(c == stop) break;
            else if(c == 8) counter -= 2;
            else input[counter] = c;
            counter++;
        }while(c != stop);
        pushMessage(makeMes(input, *me), chat);
    }else if(!strcmp(comm[0], "conf")){
            if(!strcmp(comm[1], "view")){
                if(!strcmp(comm[2], "minimal")){
                    for(char i = 8; i >= 0; i++)
                        printf("%c", CCHAT_GLOBAL_SETTINGS & 1 << i ? '1' : '0');
                    printf(" %x\n", CCHAT_GLOBAL_SETTINGS);
                }else{
                    printf("[%c] Dropping without cryptographi\n", CCHAT_GLOBAL_SETTINGS & CCHAT_FLAG_NOCRYPTO ? '*' : ' ');
                    printf("[%c] Drop only chat\n", CCHAT_GLOBAL_SETTINGS & CCHAT_FLAG_DROPOCWM ? '*' : ' ');
                    printf("[%c] Native access support\n", CCHAT_GLOBAL_SETTINGS & CCHAT_FLAG_ACCESS_E ? '*' : ' ');
                    printf("[%c] Runtime sending data\n", CCHAT_GLOBAL_SETTINGS & CCHAT_FLAG_RUNTIMEN ? '*' : ' ');
                    printf("[%c] Native audio support\n", CCHAT_GLOBAL_SETTINGS & CCHAT_FLAG_NAUDIO_E ? '*' : ' ');
                    // printf("[%c] Drop only chat\n", CCHAT_GLOBAL_SETTINGS & 1 ? '*' : ' ');
                    // printf("[%c] Drop only chat\n", CCHAT_GLOBAL_SETTINGS & 1 ? '*' : ' ');
                    // printf("[%c] Drop only chat\n", CCHAT_GLOBAL_SETTINGS & 1 ? '*' : ' ');
                }
            }else if(!strcmp(comm[1], "set")){
                if(!strcmp(comm[2], "crypto")){
                    if(*comm[3] == '0' || *comm[3] == 'n')
                        CCHAT_GLOBAL_SETTINGS |= CCHAT_FLAG_NOCRYPTO;
                    else CCHAT_GLOBAL_SETTINGS &= ~CCHAT_FLAG_NOCRYPTO;
                }else if(!strcmp(comm[2], "doc")){
                    if(*comm[3] == '0' || *comm[3] == 'n')
                        CCHAT_GLOBAL_SETTINGS &= ~CCHAT_FLAG_DROPOCWM;
                    else CCHAT_GLOBAL_SETTINGS |= CCHAT_FLAG_DROPOCWM;
                }else if(!strcmp(comm[2], "native-acc")){
                    if(*comm[3] == '0' || *comm[3] == 'n')
                        CCHAT_GLOBAL_SETTINGS &= ~CCHAT_FLAG_ACCESS_E;
                    else CCHAT_GLOBAL_SETTINGS |= CCHAT_FLAG_ACCESS_E;
                }else if(!strcmp(comm[2], "runtime-n")){
                    if(*comm[3] == '0' || *comm[3] == 'n')
                        CCHAT_GLOBAL_SETTINGS &= ~CCHAT_FLAG_RUNTIMEN;
                    else CCHAT_GLOBAL_SETTINGS |= CCHAT_FLAG_RUNTIMEN;
                }else if(!strcmp(comm[2], "native-aud")){
                    if(*comm[3] == '0' || *comm[3] == 'n')
                        CCHAT_GLOBAL_SETTINGS &= CCHAT_FLAG_NAUDIO_E;
                    else CCHAT_GLOBAL_SETTINGS |= CCHAT_FLAG_NAUDIO_E;
                }
            }
    } else if(!strcmp(comm[0], "load")){
        if(!strcmp(comm[1], "")){
            printf("No chat file. Use \':load [filename]\'\n");
            return CLI_OK_NOUT;
        }
        printf("All current chat message will destroyed\n\tAre you sure to load? [y/N] ");
        char a = getchar();
        if(a == 'y' || a == 'Y'){
            last = 0;
            chat->messages = loadMLFromFile(comm[1]);
            chat->userList = loadULFromFile("user.list");
            chat->roler = loadRolerFromFile("roles.list");
        }else printf("Aborted\n");
    }else if(!strcmp(comm[0], "help")){
        FILE *help = fopen("HELP.md", "r");
        char *link = 0x0;
        link = (char*)malloc(128);
        #ifdef __WIN32
            strcpy(link, "cmd /c start https://github.com/AlexTunder/ossa/blob/master/HELP.md");
            strcat(link, comm[1]);
        #endif
        #ifdef __linux__
            strcpy(link,"open \"https://github.com/AlexTunder/ossa/blob/master/HELP.md\"");
            strcat(link, comm[1]);
        #endif
        #ifdef __APPLE
            strcpy(link,"xdg-open \"https://github.com/AlexTunder/ossa/blob/master/HELP.md\"");
            strcat(link, comm[1]);
        #endif
        system(link);
    } 
    else {
        printf("Command \'%s\' is not found\n", comm[0]);
        return CLI_CNF;
    }
    return 0;
}
int handleCLI(struct Chat *chat, int *me){
    char y = getchar();
    if(y == '\n'){
        return 0;
    }
    if(y == ':'){
        //Handle command
        char comms[32][16];
        for(int i = 0; i < 16; i++)
            for(int j = 0; j < 32; j++)
                comms[j][i] = 0;
        char c = 0;
        int commcounter = 0;
        for( ; ; ){
            for(int j = 0; j < 32 && c != '\n' && c != ' ' ; j++){
                c = getchar();
                if(c!='\n' && c!=8 && c!=' ') comms[commcounter][j] = c;
                if(c==8) j-=2;
            }
            if(c == '\n') break;
            if(c == ' ') c = 0;
            ++commcounter;
        }
        int result = handleCommand(comms, chat, me);
        // free(comms);
        return result;
    }else if(y == '$'){
        //Handle system command
    }else{
        //Handle new message
        char c = 0;
        char input[1024];
        for(int i = 0; i < 1024; i++) input[i] = 0;
        int counter = 0;
        input[0] = y;
        do{
            c = getchar();
            if(c == '\n') break;
            else if(c == 8) counter -= 2;
            else input[++counter] = c;
        }while(c != '\n');
        pushMessage(makeMes(input, *me), chat);
        // free(input);
    }
    return 0;
} 

int main(int argc, char **argv){
    for(int i = 0; i < argc; i++){
        if(!strcmp(argv[i], "killUpdater")){
            char *toKill = (char*)malloc(1024);
            sprintf(toKill, "powershell Stop-Process -Name \"%s\" -Force", argv[i+1]);
            printf("[*]Killing updater proccess...\n");
            system(toKill);
        }
    }
    printf("========Welcome to Open Source Minimal In-line Chat (OSMIC)!========\n");
   int me = 0;
   struct Chat chat = initChat(NULL);
   chat.userList.access = 0xffffffff;
   addRole(&chat.roler, "Ban list", 0x0);
   while(1){
    for(int i = last; i < getMessagesCount(&chat); i++){
            struct Message mes = getMessage(&chat, i); 
            printf("(%i)%s@[system]: %s\n\tsended:<%I64i>\n", i, getUsernameByID(mes.userid, &chat.userList), mes.content, mes.date);
            last = i+1;
            if(last == getMessagesCount(&chat)) printf("\n");
    }
    printf("%s@[sys]: ", getUsernameByID(me, &chat.userList));
    int code = handleCLI(&chat, &me);
    if(code < CLI_ERRF) last ++;
    else if(code == CLI_EXIT){
        printf("Exit...\n");
        break;
    }else if(code == CLI_ERRF){
        printf("Fail: command error. Check all commands\n");
    }else if(code == CLI_CNF){
        printf("Command not found\n");
    }
   }
   return 0;
}