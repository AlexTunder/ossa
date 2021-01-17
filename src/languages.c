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
    /* low-level details */
    struct LMMAP{
        char *locale;
        void (*callback_load)(const char *target);
        void (*callback_fail)(const char *target, int reason);
        void (*callback_finish)(int total);
    }details;
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
    int counter = 0;
    while(fscanf(target, "%1023s", buffer) == 1){
        if(!strcmp(buffer, "yes")){ /* logic */
            if((lm->logic.yes = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("yes", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("lm->logic.yes");
        }else if(!strcmp(buffer, "no")){
            if((lm->logic.no = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("no", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("lm->logic.no");
        }else if(!strcmp(buffer, "no_acc_users")){ /* access */
            if((lm->access.no_acc_users = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("no_acc_users", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("lm->access.no_acc_users");
        }else if(!strcmp(buffer, "no_acc_evlog")){
            if((lm->access.no_acc_evlog = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("no_acc_evlog", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("no_acc_evlog");
        }else if(!strcmp(buffer, "no_acc_roler")){
            if((lm->access.no_acc_roler = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("no_acc_roler", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("no_acc_roler");
        }else if(!strcmp(buffer, "muted")){
            if((lm->access.muted = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("mutes", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("access.mutes");
        }else if(!strcmp(buffer, "ban")){
            if((lm->access.ban = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("ban", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("access.ban");
        }else if(!strcmp(buffer, "total")){ /* titles */
            if((lm->titles.total = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("total", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("titles.total");
        }else if(!strcmp(buffer, "name")){
            if((lm->titles.name = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("names", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("titles.name");
        }else if(!strcmp(buffer, "memory")){
            if((lm->titles.memory = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("memory", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("titles.memory");
        }else if(!strcmp(buffer, "role_name")){
            if((lm->titles.role_name = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("role_name", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("titles.role_name");
        }else if(!strcmp(buffer, "access_code")){
            if((lm->titles.access_code = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("access_code", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("titles.access_code");
        }else if(!strcmp(buffer, "access_number")){
            if((lm->titles.access_number = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("titles.access_number", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("titles.access_number");
        }else if(!strcmp(buffer, "bad_set")){ /* errors */
            if((lm->error.bad_set = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("errors.bad_set", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("errors.bad_set");
        }else if(!strcmp(buffer, "invalid_syntax")){
            if((lm->error.invalid_syntax = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("errors.invalid_syntax", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("errors.invalid_syntax");
        }else if(!strcmp(buffer, "no_chat_file")){
            if((lm->error.no_chat_file = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("errors.no_chat_file", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("errors.no_chat_file");
        }else if(!strcmp(buffer, "command_not_found")){
            if((lm->error.command_not_found = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("errors.command_not_found", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("errors.command_not_found");
        }else if(!strcmp(buffer, "command_error")){
            if((lm->error.command_error = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("errors.command_error", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("errors.command_errors");
        }else if(!strcmp(buffer, "user_in_role")){ /* outputs */
            if((lm->output.user_in_role = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("output.user_in_role", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("output.user_in_role");
        }else if(!strcmp(buffer, "chat_load")){
            if((lm->output.chat_load_are_you_sure = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("output.chat_load", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("output.chat_load");
        }else if(!strcmp(buffer, "exit")){
            if((lm->output.exit = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("output.exit", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("output.exit");
        }else if(!strcmp(buffer, "aborted")){
            if((lm->output.aborted = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("output.aborted", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("output.aborted");
        }else if(!strcmp(buffer, "sended")){
            if((lm->output.sended = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("output.sended", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("output.sended");
        }else if(!strcmp(buffer, "welcome")){
            if((lm->output.welcome = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("output.welcome", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("output.welcome");
        }else if(!strcmp(buffer, "locale")){
            if((lm->details.locale = readLineFromFile(target, 0)) == 0)
                if(lm->details.callback_fail != 0x0)
                    lm->details.callback_fail("locale", INVALID_SYNTAX);
            if(lm->details.callback_load != 0x0)
                lm->details.callback_load("details.locale");
        }
        else{
            if(lm->details.callback_fail != 0x0)
                lm->details.callback_fail("main", INVALID_SYNTAX);
            return counter;
        }
        counter++;
    }
    if(lm->details.callback_finish != 0x0)
        lm->details.callback_finish(counter);
    return counter;
}
