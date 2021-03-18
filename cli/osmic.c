#define AIRLIB_PATH "/"
#define ENABLE_ACCESS   
#define SETTINGS
#define OSSA_ASYNC
// #include "..\airlib\OpenCppNet\Socket.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <locale.h>
#include <sys/stat.h>
#ifndef __WIN32
 #include <unistd.h>
#else
 #include<conio.h>
#endif

// struct DynamicLanguageMap strStorage;

#include "../core/osmic.h"
#include "../core/ptcload.h"
#include "../network/api.h"

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
//COLORS
#ifndef ANSI_COLOR_RED 
    #define ANSI_COLOR_RED     "\x1b[31m"
    #define ANSI_COLOR_GREEN   "\x1b[32m"
    #define ANSI_COLOR_YELLOW  "\x1b[33m"
    #define ANSI_COLOR_BLUE    "\x1b[34m"
    #define ANSI_COLOR_MAGENTA "\x1b[35m"
    #define ANSI_COLOR_CYAN    "\x1b[36m"
    #define ANSI_COLOR_RESET   "\x1b[0m"
#endif
//Global variables
char *langPath;
int last = 0;
int currentChat = 0, lastServer = 0;
struct ChatList cl = {0x0, 0x0, 0x0, 0x0};

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
        // else printf("%s%s%s", ANSI_COLOR_RED, dlmGetTranslation(&strStorage, "no_acc_users"), ANSI_COLOR_RESET);
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
                // printf("%s%s%s", ANSI_COLOR_RED, dlmGetTranslation(&strStorage, "no_acc_evlog"), ANSI_COLOR_RESET);
                return CLI_NOPERM;
            }
        }else if(!strcmp(comm[1], "user")){
            if(getUserAccess(*me, &chat->userList) & acc_users){
                int target = atoi(comm[2]);
                if(!strcmp(comm[3], "role")){
                    if(getUserAccess(*me, &chat->userList) & acc_roler)
                        addUserToRole(&chat->roler, target, atoi(comm[4]), &chat->userList);
                    // else
                        // printf("%s%s%s", ANSI_COLOR_RED, dlmGetTranslation(&strStorage, "no_acc_roler"), ANSI_COLOR_RESET);
                } else if(!strcmp(comm[3], "ban")){
                    if(getUserAccess(*me, &chat->userList) & acc_roler)
                        addUserToRole(&chat->roler, target, 0, &chat->userList);
                    else{
                        // printf("%s%s%s", ANSI_COLOR_RED, dlmGetTranslation(&strStorage, "no_acc_roler"), ANSI_COLOR_RESET);
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
                // printf("%s%s%s", ANSI_COLOR_RED, dlmGetTranslation(&strStorage, "no_acc_users"), ANSI_COLOR_RESET);
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
                // printf("%s%s%s", ANSI_COLOR_RED, dlmGetTranslation(&strStorage, "no_acc_roler"), ANSI_COLOR_RESET);
            }
        }else if(!strcmp(comm[1], "chat")){
            currentChat = atoi(comm[2]);
            useServer(getChatChainByIndex(&cl, currentChat)->serverID);
        }
        else {printf("bad \'set\' parameter (%s)\n", comm[1]);return CLI_CNF;}
    } else if(!strcmp(comm[0], "view")){
        if(!(getUserAccess(*me, &chat->userList) & 0b01)){
            // printf("%s%s%s", ANSI_COLOR_RED, dlmGetTranslation(&strStorage, "ban"), ANSI_COLOR_RESET);
            return CLI_NOPERM;
        }
        if(!strcmp(comm[1], "all")){
            last = 0;
        }else if(!strcmp(comm[1], "tail")){
            last = getMessagesCount(chat) - atoi(comm[2]);
        }
    } else if(!strcmp(comm[0], "lsrole")){
        // printf("ID\tMemory\t\t\tRole name\tAccess code\tAccess number\n");
        // printf("ID\t%s\t\t\t%s\t%s\t%s\n", dlmGetTranslation(&strStorage, "memory"), dlmGetTranslation(&strStorage, "role_name"),dlmGetTranslation(&strStorage, "access_code"), dlmGetTranslation(&strStorage, "access_number"));
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
            // printf("%s%s%s", ANSI_COLOR_RED, dlmGetTranslation(&strStorage, "no_acc_roler"), ANSI_COLOR_RESET);
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
            printf("["ANSI_COLOR_GREEN"*"ANSI_COLOR_RESET"]Updating dist.exe\n["ANSI_COLOR_GREEN"*"ANSI_COLOR_RESET"]Killing proccess...\n");
            system("powershell Stop-Process -Name \"dist\" -Force");
            printf("["ANSI_COLOR_GREEN"*"ANSI_COLOR_RESET"]Compile....\n["ANSI_COLOR_GREEN"*"ANSI_COLOR_RESET"]");
            system("make updater");
            printf("["ANSI_COLOR_GREEN"*"ANSI_COLOR_RESET"]Enter bootstrap...\n");
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
                // char *sub = (char*)malloc(strlen(updaterCom)+32);
                // strcpy(sub, updaterCom);
                // sprintf(updaterCom, "%s -pid=%i", sub, _getpid());
                // free(sub);
            }
            system(updaterCom);
            free(updaterCom);
        }
        else if( settings & 0b001) system(".\\dist.exe");
    } else if(!strcmp(comm[0], "inrole")){
        int user_id = atoi(comm[1]);
        int role_id = atoi(comm[2]);
        if(getRoleByIndex(&chat->roler, role_id) != NULL){
            // printf("(%i)%s %s (%i)%s: ", user_id, getUsernameByID(user_id, &chat->userList), dlmGetTranslation(&strStorage, "user_in_role"), role_id, getRoleByIndex(&chat->roler, role_id)->name);
            // if(checkInRole(getRoleByIndex(&chat->roler, role_id), user_id))
            //     printf("%s\n", dlmGetTranslation(&strStorage, "yes"));
            // else printf("%s\n", dlmGetTranslation(&strStorage, "no"));
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
                    printf("["ANSI_COLOR_GREEN"%c"ANSI_COLOR_RESET"] Dropping without cryptographi\n", CCHAT_GLOBAL_SETTINGS & CCHAT_FLAG_NOCRYPTO ? '*' : ' ');
                    printf("["ANSI_COLOR_YELLOW"%c"ANSI_COLOR_RESET"] Drop only chat\n", CCHAT_GLOBAL_SETTINGS & CCHAT_FLAG_DROPOCWM ? '*' : ' ');
                    #ifdef DISABLE_ACCESS
                     printf("["ANSI_COLOR_GREEN"%c"ANSI_COLOR_RESET"] Native access support\n", CCHAT_GLOBAL_SETTINGS & CCHAT_FLAG_ACCESS_E ? '*' : ' ');
                    #else
                     printf("["ANSI_COLOR_CYAN"*"ANSI_COLOR_RESET"] Native access support\n");
                    #endif
                    #ifdef OSSA_ASYNC
                     printf("["ANSI_COLOR_CYAN"*"ANSI_COLOR_RESET"] Async mode\n");
                    #else
                     printf("["ANSI_COLOR_CYAN" "ANSI_COLOR_RESET"] Async mode\n");
                    #endif
                    printf("["ANSI_COLOR_GREEN"%c"ANSI_COLOR_RESET"] Runtime sending data\n", CCHAT_GLOBAL_SETTINGS & CCHAT_FLAG_RUNTIMEN ? '*' : ' ');
                    printf("["ANSI_COLOR_GREEN"%c"ANSI_COLOR_RESET"] Native audio support\n", CCHAT_GLOBAL_SETTINGS & CCHAT_FLAG_NAUDIO_E ? '*' : ' ');
                    printf("["ANSI_COLOR_RED"*"ANSI_COLOR_RESET"] Runtime sending data\n"/*, CCHAT_GLOBAL_SETTINGS & CCHAT_FLAG_DYNLANMP ? '*' : ' '*/);
                    //Descriptions
                    printf("\n["ANSI_COLOR_CYAN"*"ANSI_COLOR_RESET"] - already compiled; enables in code level\n["ANSI_COLOR_GREEN"*"ANSI_COLOR_RESET"] - enabled flag\n["\
                    ANSI_COLOR_RED"*"ANSI_COLOR_RESET"] - disabled on code level\n["ANSI_COLOR_YELLOW"*"ANSI_COLOR_RESET"] - Enabled, but not supported (blocked)\n");
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
                    #ifdef DISABLE_ACCESS
                    if(*comm[3] == '0' || *comm[3] == 'n')
                        CCHAT_GLOBAL_SETTINGS &= ~CCHAT_FLAG_ACCESS_E;
                    else CCHAT_GLOBAL_SETTINGS |= CCHAT_FLAG_ACCESS_E;
                    #else
                     printf(ANSI_COLOR_YELLOW"Unable to change config: soft-const config\n"ANSI_COLOR_RESET);
                    #endif
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
            // printf("%s \':load [filename]\'\n", dlmGetTranslation(&strStorage, "no_chat_file"));
            return CLI_OK_NOUT;
        }
        // printf("%s [y/N] ", dlmGetTranslation(&strStorage, "chat_load"));
        char a = getchar();
        if(a == 'y' || a == 'Y'){
            last = 0;
            chat->messages = loadMLFromFile(comm[1]);
            chat->userList = loadULFromFile("user.list");
            chat->roler = loadRolerFromFile("roles.list");
        }//else printf("%s\n", dlmGetTranslation(&strStorage, "aborted"));
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
    } else if(!strcmp(comm[0], "rm")){
        if(!strcmp(comm[1], "user")){
            struct UserList *ul = getUserById(&chat->userList, atoi(comm[2]));
            if(ul != NULL){
                free(ul->name);
                getUserById(&chat->userList, atoi(comm[2]) - 1)->next = ul->next;
                free(ul);
            }
        }else if(!strcmp(comm[1], "role")){
            // TODO: code this. I don't want to do it now
        }else if(!strcmp(comm[1], "message")){
            struct MessageList *ml = getMLFromML(&chat->messages, atoi(comm[2]));
            if(ml != 0x0){
                free(ml->me.content);
                getMLFromML(&chat->messages, atoi(comm[2]))->next = ml->next;
                free(ml);
            }
        }
    } else if(!strcmp(comm[0], "conn")){
        //conn[1] - IP conn[2] - port
        if(!strcmp(comm[1], "") || !strcmp(comm[2], "")){
            // No ip or port
            return -1;
        }
        int server_access = setServer(comm[1], atoi(comm[2]));
        if (server_access == OSSA_AUTH_FREE_MAC){
            {printf(ANSI_COLOR_GREEN"\nConnected"ANSI_COLOR_RESET" (MAC-open chat)\nUser ID: %i\n", authServer("",""));
                getChatChainByIndex(&cl, currentChat)->serverID = lastServer++;}
        }else if(server_access == OSSA_AUTH_FREE_IP4){
            {printf(ANSI_COLOR_GREEN"\nConnected"ANSI_COLOR_RESET" (IPv4-open chat)\nUser ID: %i\n", authServer("",""));
                getChatChainByIndex(&cl, currentChat)->serverID = lastServer++;}
        }else if(server_access == OSSA_AUTH_PWD_MAC){
            printf(ANSI_COLOR_GREEN"Accepted"ANSI_COLOR_RESET" (MAC-Chat with password)\nPassword: ");
            char c = 0;
            char *pwd = (char*)malloc(1024);
            for (int i = 0; i < 1024; i++){
                c = getchar();
                if(c == '\n') break;
                else pwd[i] = c;
            }
            server_access = authServer("", pwd);
            if(server_access < 0){
                printf(ANSI_COLOR_RED"\nFailed to login chat"ANSI_COLOR_RESET": wrong password\n");
                closeServer(lastServer);
                return -1;
            }else{
                printf(ANSI_COLOR_GREEN"\nConnected"ANSI_COLOR_RESET"\nUser ID: %i\n", server_access);
                getChatChainByIndex(&cl, currentChat)->serverID = lastServer++;
            }
        }else if(server_access == OSSA_AUTH_PWD_UN){
            printf(ANSI_COLOR_GREEN"Accepted"ANSI_COLOR_RESET" (Usernames-Chat with password)\nUsername: ");
            char c = 0;
            char *uname = (char*)malloc(1024);
            for (int i = 0; i < 1024; i++){
                c = getchar();
                if(c == '\n') {uname[i] = 0; break;}
                else uname[i] = c;
            }
            printf("Password: ");
            c = 0;
            // char *pwd = (char*)malloc(1024);
            #ifndef __WIN32
            char *pwd = getpass("");
            #else
            char *pwd = (char*)malloc(1024);
            for(int i = 0; i < 1024; i++)
                pwd[i] = 0;
            for (int i = 0; i < 1024; i++){
                c = getch();
                if(c == 13) {pwd[i] = 0; break;}
                else pwd[i] = c;
            }
            #endif
            server_access = authServer(uname, pwd);
            if(server_access < 0){
                printf(ANSI_COLOR_RED"\nFailed to login chat"ANSI_COLOR_RESET": wrong username or password (%i)\n", server_access);
                closeServer(lastServer);
                return -1;
            }else{
                printf(ANSI_COLOR_GREEN"\nConnected"ANSI_COLOR_RESET"\nUser ID: %i\n", server_access);
                getChatChainByIndex(&cl, currentChat)->serverID = lastServer++;
                // getChatChainByIndex(&cl, currentChat)->chat.messages
                printf("Loading messages...\n");
                syncMessages(&getChatChainByIndex(&cl, currentChat)->chat.messages);
                printf("Loading users...\n");
                syncUsers(&getChatChainByIndex(&cl, currentChat)->chat.userList);
                printf("Loading roles...\n");
                syncRoles(&getChatChainByIndex(&cl, currentChat)->chat.roler);
            }
        }else{
            printf(ANSI_COLOR_RED"Failed to connect"ANSI_COLOR_RESET": irregular answer, error code: %i\n", server_access);
        }
    } else if(!strcmp(comm[0], "mkchat")){
        int index = pushChatToCL(&cl, initChat(0x0));
        getChatChainByIndex(&cl, index)->name = strvp(comm[1]);
    } else if(!strcmp(comm[0], "lschat")){
        printf("(ID)\tName\tUsers\tMessage\n");
        for(int i = 0; i < getChatChainLen(&cl); i++){
            printf(
                "%s(%c)"ANSI_COLOR_RESET" %i\t%s\t%i\t%i\n",
                getChatChainByIndex(&cl, i)->serverID!=-1?ANSI_COLOR_GREEN:ANSI_COLOR_YELLOW,
                currentChat == i ? '*':'.',
                i,
                getChatChainByIndex(&cl, i)->name,
                getUsersCount(&getChatChainByIndex(&cl, i)->chat)-1, /*Without system user*/
                getMessagesCount(&getChatChainByIndex(&cl, i)->chat)-1 /*Without open-chat message*/
            );
        }
    } else if(!strcmp(comm[0], "disconn")){
        if(getChatChainByIndex(&cl, currentChat)->serverID != -1){
            closeServer(getChatChainByIndex(&cl, currentChat)->serverID);
            getChatChainByIndex(&cl, currentChat)->serverID = -1;
        }
    } else if(!strcmp(comm[0], "dload")){
        if(!strcmp(comm[1], "proto")){
            //Downloading proto from https://raw.githubusercontent.com/AlexTunder/ossa-ptc/main/[proto]
            // curl [link] -o [target]
            char *url = (char*)malloc(strlen("curl https://raw.githubusercontent.com/AlexTunder/ossa-ptc/main/ -o ")+strlen(comm[2])+strlen(comm[2])+10);
            #ifndef __WIN32
             sprintf(url, "curl https://raw.githubusercontent.com/AlexTunder/ossa-ptc/main/%s.so -o %s.so", comm[2], comm[2]);
            #else
             sprintf(url, "curl https://raw.githubusercontent.com/AlexTunder/ossa-ptc/main/%s.dll -o %s.dll", comm[2], comm[2]);
            #endif
            system(url);
        }else if(!strcmp(comm[1], "lang")){
            char *url = (char*)malloc(strlen("curl https://raw.githubusercontent.com/AlexTunder/ossa/master/languages/ -o ")+strlen(comm[2])+strlen(comm[2])+10);
            sprintf(url, "curl https://raw.githubusercontent.com/AlexTunder/ossa/master/languages/%s.lang -o %s.lang", comm[2], comm[2]);
            system(url);
        }else{
            printf(ANSI_COLOR_RED"invalid tag"ANSI_COLOR_RESET": %s\n", comm[1]);
        }
    } else if(!strcmp("env-ctl", comm[0])){
        if(!strcmp("update", comm[1])){
            // loadDLMFromFile(langPath, &strStorage);
        }else if(!strcmp("list", comm[1])){
            //List all enviroments
        }
    } else if(!strcmp("iload", comm[0])){
        if(!strcmp("proto", comm[1])){
            printf("loading protocol \'%s\'...\n");
            struct stat buffer;

            stat(comm[2], &buffer);
            if(buffer.st_mode & S_IFDIR){
                // printf("unzipping proto...\n");
                // char *envmap = (char *)malloc(strlen(comm[2]) + strlen("proto.env"));
                // char *dllmap = (char *)malloc(strlen(comm[2]) + strlen("proto.dll"));
                // char *cmdmap = (char *)malloc(strlen(comm[2]) + strlen("comms.cmdkey"));
                // sprintf(envmap, "%s/proto.env");
                // sprintf(dllmap, "%s/proto.dll");
                // sprintf(cmdmap, "%s/comms.cmdkey");
                // loadDLMFromFile()
            }
            //Load shared lib here!
        }
    }
    else {
        // printf("%s (\'%s\')\n", dlmGetTranslation(&strStorage, "command_not_found"), comm[0]);
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
        if(!(getUserAccess(*me, &chat->userList) & 0b10)){
            // printf("%s%s%s", ANSI_COLOR_RED, dlmGetTranslation(&strStorage, "muted"), ANSI_COLOR_RESET);
            return CLI_NOPERM;
        }
        pushMessage(makeMes(input, *me), chat);
        // free(input);
    }
    return 0;
}

int main(int argc, char **argv){
    /* Languages */
    #ifdef __WIN32
    //  system("chcp 65001"); //Setting up UTF-8!
    #endif
    /* default settings */
    langPath = (char*)malloc(10240);
    sprintf(langPath, "default.lang");
    /* parsing arguments */
    for(int i = 0; i < argc; i++){
        if(!strcmp(argv[i], "killUpdater")){
            char *toKill = (char*)malloc(1024);
            sprintf(toKill, "powershell Stop-Process -Name \"%s\" -Force", argv[i+1]);
            printf("[*]Killing updater proccess...\n");
            system(toKill);
        }else if(!strcmp(argv[i], "--lang")){
            free(langPath);
            langPath = argv[++i];
        }
    }
    printf("loading langs...\n");
    // loadDLMFromFile(langPath, &strStorage);
    printf("loaded langs...\n");
    // printf("locale: \'%s\'\n", strStorage.details.locale);
    // setlocale(LC_ALL, dlmGetTranslation(&strStorage, "locale"));
    // printf("%s\n", dlmGetTranslation(&strStorage, "welcome")); //Welcome!
    /* main code */
   int me = 0;
   cl.chat = initChat(0x0);
   cl.chat.userList.access = 0xffffffff;
   cl.chat.roler.role.access = 0xffffffff;
   cl.name = strvp("local");
   cl.serverID = -1;
   addUserToRole(&cl.chat.roler, 0, 0, &cl.chat.userList);
   addRole(&cl.chat.roler, "Ban list", 0x0);
   while(1){
       struct ChatList *cur = getChatChainByIndex(&cl, currentChat);
       for(int i = last; i < getMessagesCount(&cur->chat); i++){
               struct Message mes = getMessage(&cur->chat, i); 
               printf("(%i)%s@[%s]: %s\n\t%s:<%I64i>\n", i, getUsernameByID(mes.userid, &cur->chat.userList), cur->name, mes.content, "sended:" ,mes.date);
               last = i+1;
               if(last == getMessagesCount(&cur->chat)) printf("\n");
       }
    printf("%s@[%s]: ", getUsernameByID(me, &cur->chat.userList), cur->name);
    int code = handleCLI(&cur->chat, &me);
    if(code < CLI_ERRF) last ++;
    else if(code == CLI_EXIT){
        // printf("%s\n", dlmGetTranslation(&strStorage, "exit"));
        // destroyUL(&chat.userList);
        for(int i = 0; i < getChatChainLen(&cl); i++){
            closeServer(i);
        }
        break;
    }else if(code == CLI_ERRF){
        printf("Fail: command error. Check all commands\n");
    }else if(code == CLI_CNF){
        
    }
   }
   return 0;
}