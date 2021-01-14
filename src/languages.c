#pragma once
#ifndef fprintf
 #include <stdio.h>
#endif
#ifndef malloc
 #include <stdlib.h>
#endif
#define NO_ERR          0
#define NO_FILE         -1
#define INVALID_SYNTAX  -2
#ifndef ANSI_COLOR_RED 
    #define ANSI_COLOR_RED     "\x1b[31m"
    #define ANSI_COLOR_GREEN   "\x1b[32m"
    #define ANSI_COLOR_YELLOW  "\x1b[33m"
    #define ANSI_COLOR_BLUE    "\x1b[34m"
    #define ANSI_COLOR_MAGENTA "\x1b[35m"
    #define ANSI_COLOR_CYAN    "\x1b[36m"
    #define ANSI_COLOR_RESET   "\x1b[0m"
#endif
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
        // printf("====== %i ======\nBuffer 1: %s\nBuffer 2: %s\n",i, buffer1, buffer2);
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
        printf("[!]%s%s \'%s\'%s\n", ANSI_COLOR_RED, "No language package. Entering (nulli-a) mode. download file from link below:\n\t", path, ANSI_COLOR_RESET"https://github.com/AlexTunder/ossa/blob/master/default.lang\n");
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
            printf("[*] \"strStorage.logic.no\" loaded.\n");
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
