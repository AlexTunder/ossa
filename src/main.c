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

//ErrorsCodes
#define NO_ERR          0
#define NO_FILE         -1
#define INVALID_SYNTAX  -2
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
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//Global variables
int last = 0;

struct langMap{
    /* logic */
    struct LANMAP_LOGIC{ char
        *yes,
        *no;
    }logic;
    /* No access strings */
    struct LANMAP_ERR_ACC{ char
        *no_acc_users,
        *no_acc_evlog,
        *no_acc_roler,
        *muted,
        *ban;
    }access;
    /* Table titles */
    struct LANMAP_TITLES{ char
        *total,
        *name,
        *memory,
        *role_name,
        *access_code,
        *access_number;
    }titles;
    /* error */
    struct LANMAP_ERR{ char
        *bad_set,
        *invalid_syntax,
        *no_chat_file,
        *command_not_found,
        *command_error;
    }error;
    /* ok output */
    struct LANMAP_OK{ char
        *user_in_role,
        *chat_load_are_you_sure,
        *exit,
        *aborted,
        *sended,
        *welcome;
    }output;
}strStorage;

char *readLineFromFile(FILE *file, int *size){
    char c = 0,
    /* double buffering, cuz we don't know size of word */
    *buffer1 = 0x0,
    *buffer2 = 0x0;
    int i = 0;

    //MAGIC HERE!
    buffer1 = (char*)malloc(1);
    *buffer1 = 0;
    buffer2 = (char*)malloc(1);
    *buffer2 = 0;

    for(; fread(&c, 1, 1, file) != 0; i++){
        if(c == '\n' || c == '\r')
            break;
        if(i%2 == 0){
            free(buffer1);
            buffer1 = (char*)malloc(i+2);
            sprintf(buffer1, "%s%c", buffer2, c);
        }else{
            free(buffer2);
            buffer2 = (char*)malloc(i+2);
            sprintf(buffer2, "%s%c", buffer1, c);
        }
        printf("====== %i ======\nBuffer 1: %s\nBuffer 2: %s\n",i, buffer1, buffer2);
    }

    if(size != 0x0)
        *size = i;
    if(strlen(buffer1) > strlen(buffer2))
        {free(buffer2);return buffer1;}
    else 
        {free(buffer1);return buffer2;}
}

int loadLMFromFile(const char *path, struct langMap *lm){
    FILE *target = fopen(path, "r");
    if(target == 0x0){
        return NO_FILE;
    }
    char buffer[1024];
    while(fscanf(target, "%1023s", buffer) == 1){
        if(!strcmp(buffer, "yes")){ /* logic */
            if((strStorage.logic.yes = readLineFromFile(target, 0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "no")){
            if((strStorage.logic.no = readLineFromFile(target, 0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "no_acc_users")){ /* access */
            if((strStorage.access.no_acc_users = readLineFromFile(target, 0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "no_acc_evlog")){
            if((strStorage.access.no_acc_evlog = readLineFromFile(target, 0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "no_acc_roler")){
            if((strStorage.access.no_acc_roler = readLineFromFile(target, 0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "muted")){
            if((strStorage.access.muted = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "ban")){
            if((strStorage.access.ban = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "total")){ /* titles */
            if((strStorage.titles.total = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "name")){
            if((strStorage.titles.name = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "memory")){
            if((strStorage.titles.memory = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "role_name")){
            if((strStorage.titles.role_name = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "access_code")){
            if((strStorage.titles.access_code = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "access_number")){
            if((strStorage.titles.access_number = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "bad_set")){ /* errors */
            if((strStorage.error.bad_set = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "invalid_syntax")){
            if((strStorage.error.invalid_syntax = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "no_chat_file")){
            if((strStorage.error.no_chat_file = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "command_not_found")){
            if((strStorage.error.command_not_found = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "command_error")){
            if((strStorage.error.command_error = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "user_in_role")){ /* outputs */
            if((strStorage.output.user_in_role = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "chat_load")){
            if((strStorage.output.chat_load_are_you_sure = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "exit")){
            if((strStorage.output.exit = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "aborted")){
            if((strStorage.output.aborted = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "sended")){
            if((strStorage.output.sended = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else if(!strcmp(buffer, "welcome")){
            if((strStorage.output.welcome = readLineFromFile(target, 0x0)) == 0)
                printf("%s%s \'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, path, ANSI_COLOR_RESET);
            printf("[*] \"strStorage.logic.yes\" loaded.\n");
        }else{
            printf("%s%s (Unexpected ID - \"%s\")\'%s\'%s\n", ANSI_COLOR_RED, strStorage.error.invalid_syntax, buffer, path, ANSI_COLOR_RESET);
        }
    }
    printf("[*] Loading finished\n");
    return 0;
}

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
        else printf("%s%s%s", ANSI_COLOR_RED, strStorage.access.no_acc_users, ANSI_COLOR_RESET);
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
                printf("%s%s%s", ANSI_COLOR_RED, strStorage.access.no_acc_evlog, ANSI_COLOR_RESET);
                return CLI_NOPERM;
            }
        }else if(!strcmp(comm[1], "user")){
            if(getUserAccess(*me, &chat->userList) & acc_users){
                int target = atoi(comm[2]);
                if(!strcmp(comm[3], "role")){
                    if(getUserAccess(*me, &chat->userList) & acc_roler)
                        addUserToRole(&chat->roler, target, atoi(comm[4]), &chat->userList);
                    else{
                        printf("%s%s%s", ANSI_COLOR_RED, strStorage.access.no_acc_roler, ANSI_COLOR_RESET);
                    }
                } else if(!strcmp(comm[3], "ban")){
                    if(getUserAccess(*me, &chat->userList) & acc_roler)
                        addUserToRole(&chat->roler, target, 0, &chat->userList);
                    else{
                        printf("%s%s%s", ANSI_COLOR_RED, strStorage.access.no_acc_roler, ANSI_COLOR_RESET);
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
                printf("%s%s%s", ANSI_COLOR_RED, strStorage.access.no_acc_users, ANSI_COLOR_RESET);
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
                printf("%s%s%s", ANSI_COLOR_RED, strStorage.access.no_acc_roler, ANSI_COLOR_RESET);
            }
        }
        else {printf("bad \'set\' parameter (%s)\n", comm[1]);return CLI_CNF;}
    } else if(!strcmp(comm[0], "view")){
        if(!(getUserAccess(me, &chat->userList) & 0b01)){
            printf("%s%s%s", ANSI_COLOR_RED, strStorage.access.ban, ANSI_COLOR_RESET);
            return CLI_NOPERM;
        }
        if(!strcmp(comm[1], "all")){
            last = 0;
        }else if(!strcmp(comm[1], "tail")){
            last = getMessagesCount(chat) - atoi(comm[2]);
        }
    } else if(!strcmp(comm[0], "lsrole")){
        // printf("ID\tMemory\t\t\tRole name\tAccess code\tAccess number\n");
        printf("ID\t%s\t\t\t%s\t%s\t%s\n", strStorage.titles.memory, strStorage.titles.role_name, strStorage.titles.access_code, strStorage.titles.access_number);
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
            printf("%s%s%s", ANSI_COLOR_RED, strStorage.access.no_acc_roler, ANSI_COLOR_RESET);
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
            printf("(%i)%s %s (%i)%s: ", user_id, getUsernameByID(user_id, &chat->userList), strStorage.output.user_in_role, role_id, getRoleByIndex(&chat->roler, role_id)->name);
            if(checkInRole(getRoleByIndex(&chat->roler, role_id), user_id))
                printf("%s\n", strStorage.logic.yes);
            else printf("%s\n", strStorage.logic.no);
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
                    printf("["ANSI_COLOR_GREEN"%c"ANSI_COLOR_RESET"] Drop only chat\n", CCHAT_GLOBAL_SETTINGS & CCHAT_FLAG_DROPOCWM ? '*' : ' ');
                    printf("["ANSI_COLOR_GREEN"%c"ANSI_COLOR_RESET"] Native access support\n", CCHAT_GLOBAL_SETTINGS & CCHAT_FLAG_ACCESS_E ? '*' : ' ');
                    printf("["ANSI_COLOR_GREEN"%c"ANSI_COLOR_RESET"] Runtime sending data\n", CCHAT_GLOBAL_SETTINGS & CCHAT_FLAG_RUNTIMEN ? '*' : ' ');
                    printf("["ANSI_COLOR_GREEN"%c"ANSI_COLOR_RESET"] Native audio support\n", CCHAT_GLOBAL_SETTINGS & CCHAT_FLAG_NAUDIO_E ? '*' : ' ');
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
            printf("%s \':load [filename]\'\n", strStorage.error.no_chat_file);
            return CLI_OK_NOUT;
        }
        printf("%s [y/N] ", strStorage.output.chat_load_are_you_sure);
        char a = getchar();
        if(a == 'y' || a == 'Y'){
            last = 0;
            chat->messages = loadMLFromFile(comm[1]);
            chat->userList = loadULFromFile("user.list");
            chat->roler = loadRolerFromFile("roles.list");
        }else printf("%s\n", strStorage.output.aborted);
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
    }
    else {
        printf("%s (\'%s\')\n", strStorage.error.command_not_found, comm[0]);
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
        if(!(getUserAccess(me, &chat->userList) & 0b10)){
            printf("%s%s%s", ANSI_COLOR_RED, strStorage.access.muted, ANSI_COLOR_RESET);
            return CLI_NOPERM;
        }
        pushMessage(makeMes(input, *me), chat);
        // free(input);
    }
    return 0;
} 

int main(int argc, char **argv){
    printf("Loading your default language...\n");
    loadLMFromFile("default.lang", &strStorage);
    for(int i = 0; i < argc; i++){
        if(!strcmp(argv[i], "killUpdater")){
            char *toKill = (char*)malloc(1024);
            sprintf(toKill, "powershell Stop-Process -Name \"%s\" -Force", argv[i+1]);
            printf("[*]Killing updater proccess...\n");
            system(toKill);
        }
    }
    printf("%s\n", strStorage.output.welcome); //Welcome!
   int me = 0;
   struct Chat chat = initChat(NULL);
   chat.userList.access = 0xffffffff;
   chat.roler.role.access = 0xffffffff;
   addUserToRole(&chat.roler, 0, 0, &chat.userList);
   addRole(&chat.roler, "Ban list", 0x0);
   while(1){
    for(int i = last; i < getMessagesCount(&chat); i++){
            struct Message mes = getMessage(&chat, i); 
            printf("(%i)%s@[system]: %s\n\t%s:<%I64i>\n", i, getUsernameByID(mes.userid, &chat.userList), mes.content, strStorage.output.sended ,mes.date);
            last = i+1;
            if(last == getMessagesCount(&chat)) printf("\n");
    }
    printf("%s@[sys]: ", getUsernameByID(me, &chat.userList));
    int code = handleCLI(&chat, &me);
    if(code < CLI_ERRF) last ++;
    else if(code == CLI_EXIT){
        printf("%s\n", strStorage.output.exit);
        break;
    }else if(code == CLI_ERRF){
        printf("Fail: command error. Check all commands\n");
    }else if(code == CLI_CNF){
        printf("Command not found\n");
    }
   }
   return 0;
}