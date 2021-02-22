#pragma once
#include "./types.h"
#include "./base.h"
#include "./nubis.h"

#ifndef CCHAT_FILEWORKS_C
#define CCHAT_FILEWORKS_C
#define CCHAT_FILEWORKS_H

#ifndef CCHAT_RM_STREAM
struct MessageList loadMLFromStream(const char *str){
    struct NCStream target = makeStream(str, strlen(str));
    char *mesBuf = (char*)malloc(5120);
    struct MessageList me = initML();
    char c = 0;
    char err[1024];
    while(1){ /*read one message*/
        if(csread(&c, 1, 1, target) == 0) break;
        //Syntax check
        if(c != 1){
            sprintf(err, "Failed to read chat from stream: corrupted syntax (no HEAD)");
            pushMessageToML(makeMes(err, 0), &me);
            return me;
        }
        struct Message sub = makeMes("", 0);
        csread(&sub.userid, sizeof(sub.userid), 1, target);
        csread(&sub.date, sizeof(sub.date), 1, target);
        csread(&c, 1, 1, target);
        //Syntax check
        if(c!=2){
            sprintf(err, "Failed to read chat from stream: corrupted syntax (no BODY-START)");
            pushMessageToML(makeMes(err, 0), &me);
            return me;
        }
        sprintf(mesBuf, "\0");
        csread(&c, 1, 1, target);
        do{
            sprintf(mesBuf, "%s%c", mesBuf, c);
            if(csread(&c, 1, 1, target) == 0) break;
        }while(c!=3);
        sprintf(mesBuf, "%s%c", mesBuf, 0);
        sub.content = (char*)malloc(strlen(mesBuf));
        strcpy(sub.content, mesBuf);
        pushMessageToML(sub, &me);
    }
    free(mesBuf);
    return me;
}
struct UserList loadULFromStream(const char *str){
    struct NCStream target = makeStream(str, strlen(str));
    struct UserList me = initUserList();
    char *name = (char*)malloc(32);
    for(int i = 0; i < 32; i++)
        name[i] = 0;
    char c = 0;
    while(csread(&c, 1, 1, target)){
        if(c == '\n'){
            if(*name != 0)
                if(strcmp(name, "system")) pushUserToUL(name, &me);
            for(int i = 0; i < 32; i++)
                name[i] = 0;
        }
        else sprintf(name, "%s%c", name, c);
    }
    return me;
}
struct Roler loadRolerFromStream(const char *str){
    struct NCStream target = makeStream(str, strlen(str));
    struct Roler me = initRoler();
    char c = 0, *name = (char*)malloc(32);
    for(long roleid = 0; ; roleid++){
        struct Role this;
        // if(fread(&c, 1, 1, target) == 0) return me;
        // else fseek(target, SEEK_CUR-1, 0);
        for(int i = 0; c != '\t'; i++){
            if(csread(&c, 1, 1, target) == 0) return me;
            else if(i >= 32){
                fprintf(stderr, "Fatal error: invelid syntax in %p stream. rolename is too long (should be less then 32)\n", target);
            }
            else if(c != '\t')
                name[i] = c;
        }
        this.name = (char*)malloc(strlen(name));
        strcpy(this.name, name);
        for(int i = 0; i < 32; i++)
            name[i] = 0;
        if(c != '\t'){
            fprintf(stderr, "Fatal error: invelid syntax in %p stream. \' \' is skipped, stage 0. unexpected: \'%c\'(%i)\n", target, c, c);
            return initRoler();
        }
        csread(&this.access, 4, 1, target);
        csread(&c, 1, 1, target);
        if(c != '\t'){
            fprintf(stderr, "Fatal error: invelid syntax in %p stream. \' \' is skipped, stage 1. unexpected: \'%c\'(%i)\n", target, c, c);
            return initRoler();
        }
        csread(&c, 1, 1, target);
        if(c != '{'){
            fprintf(stderr, "Fatal error: invelid syntax in %p stream. \'{\' is skipped, stage 3. unexpected: \'%c\'(%i)\n", target, c, c);
            return initRoler();
        }
        addRole(&me, this.name, this.access);
        int read = 0;
        while(csread(&c, 1, 1, target) != 0){ // Reading all userlinks for this role
            if(c == '}') break;
            else{
                csseek(target, -1, SEEK_CUR);
                csread(&read, 4, 1, target);
                addUserToRole(&me, read, roleid, NULL);
            }
        }
    }
    return me;
}
#endif
struct MessageList loadMLFromFile(const char *filename){
    struct MessageList me = initML();
    #include "fbase.h"
    target = fopen(filename, "r");
    if(target == 0x0){
        printf("Failed to open file \'%s\' (reading MessageList)\n", filename);
        fclose(target);
        return me;
    }
    char c = 0;
    char err[1024];
    while(1){ /*read one message*/
        if(fread(&c, 1, 1, target) == 0) break;
        //Syntax check
        if(c != 1){
            sprintf(err, "Failed to read chat from file: corrupted syntax (no HEAD)");
            pushMessageToML(makeMes(err, 0), &me);
            fclose(target);
            return me;
        }
        struct Message sub = makeMes("", 0);
        fread(&sub.userid, sizeof(sub.userid), 1, target);
        fread(&sub.date, sizeof(sub.date), 1, target);
        fread(&c, 1, 1, target);
        //Syntax check
        if(c!=2){
            sprintf(err, "Failed to read chat from file: corrupted syntax (no BODY-START)");
            pushMessageToML(makeMes(err, 0), &me);
            fclose(target);
            return me;
        }
        sprintf(mesBuf, "\0");
        fread(&c, 1, 1, target);
        do{
            sprintf(mesBuf, "%s%c", mesBuf, c);
            if(fread(&c, 1, 1, target) == 0) break;
        }while(c!=3);
        sprintf(mesBuf, "%s%c", mesBuf, 0);
        sub.content = (char*)malloc(strlen(mesBuf));
        strcpy(sub.content, mesBuf);
        pushMessageToML(sub, &me);
    }
    fclose(target);
    free(mesBuf);
    return me;
}
struct UserList loadULFromFile(const char *filename){
    struct UserList me = initUserList();
    FILE *target = fopen(filename, "r");
    if(target == 0x0){
        printf("Failed to open file \'%s\' (reading MessageList)\n", filename);
        fclose(target);
        return me;
    }
    char *name = (char*)malloc(32);
    for(int i = 0; i < 32; i++)
        name[i] = 0;
    char c = 0;
    while(fread(&c, 1, 1, target)){
        if(c == '\n'){
            if(*name != 0)
                if(strcmp(name, "system")) pushUserToUL(name, &me);
            for(int i = 0; i < 32; i++)
                name[i] = 0;
        }
        else sprintf(name, "%s%c", name, c);
    }
    fclose(target);
    return me;
}
struct Roler loadRolerFromFile(const char *filename){
    struct Roler me = initRoler();
    FILE *target = fopen(filename, "r");
    if(target == 0x0){
        fprintf(stderr, "Failed to open file \'%s\' (reading Roler)\n", filename);
        fclose(target);
        return me;
    }
    char c = 0, *name = (char*)malloc(32);
    for(long roleid = 0; ; roleid++){
        struct Role this;
        // if(fread(&c, 1, 1, target) == 0) return me;
        // else fseek(target, SEEK_CUR-1, 0);
        for(int i = 0; c != '\t'; i++){
            if(fread(&c, 1, 1, target) == 0) {fclose(target);return me;}
            else if(i >= 32){
                fprintf(stderr, "Fatal error: invelid syntax in %s file. rolename is too long (should be less then 32)\n", filename);
            }
            else if(c != '\t')
                name[i] = c;
        }
        this.name = (char*)malloc(strlen(name));
        strcpy(this.name, name);
        for(int i = 0; i < 32; i++)
            name[i] = 0;
        if(c != '\t'){
            fprintf(stderr, "Fatal error: invelid syntax in \'%s\' file. \' \' is skipped, stage 0. unexpected: \'%c\'(%i)\n", filename, c, c);
            fclose(target);
            return initRoler();
        }
        fread(&this.access, 4, 1, target);
        fread(&c, 1, 1, target);
        if(c != '\t'){
            fprintf(stderr, "Fatal error: invelid syntax in \'%s\' file. \' \' is skipped, stage 1. unexpected: \'%c\'(%i)\n", filename, c, c);
            fclose(target);
            return initRoler();
        }
        fread(&c, 1, 1, target);
        if(c != '{'){
            fprintf(stderr, "Fatal error: invelid syntax in \'%s\' file. \'{\' is skipped, stage 3. unexpected: \'%c\'(%i)\n", filename, c, c);
            fclose(target);
            return initRoler();
        }
        addRole(&me, this.name, this.access);
        int read = 0;
        while(fread(&c, 1, 1, target) != 0){ // Reading all userlinks for this role
            if(c == '}') break;
            else{
                fseek(target, -1, SEEK_CUR);
                fread(&read, 4, 1, target);
                addUserToRole(&me, read, roleid, NULL);
            }
        }
    }
    fclose(target);
    return me;
}
int dropChatToFile(struct Chat *chat, char *fn_c, char *fn_u, char *fn_r){
    char *fn;
    region{ /* drop userlist */
        if(fn_u == 0x0){ //by default using user.list
            fn = (char*)malloc(1024);
            sprintf(fn, "%s/%s", chat->syncer.path, "user.list");
        } else fn = fn_u;
        FILE *fd = fopen(fn, "w");
        int max = getUsersCount(chat);
        for(int i = 0; i < max; i++)
            fprintf(fd, "%s\n", getUsernameByID(i, &chat->userList));
        fflush(fd);
        fclose(fd);
        if(fn_u == 0x0) free(fn);
    }
    region{ /* drop messagelist */
        if(fn_c == 0x0){//by default using ctime of first and last messages
            fn = (char*)malloc(1024);
            sprintf(fn, "%s/%I64i-%I64i", chat->syncer.path, chat->messages.me.date, getLastFromChat(chat).date);
        } else fn = fn_c;
        FILE *fd = fopen(fn, "w");
        int max = getMessagesCount(chat);
        struct MessageList *counter = &chat->messages;
        for(int i = 0; i < max; i++){
            fprintf(fd, "%c", 1);
            fwrite(&counter->me.userid, sizeof(counter->me.userid), 1, fd);
            fwrite(&counter->me.date, sizeof(counter->me.date), 1, fd);
            fprintf(fd, "%c", 2);
            for(long long i = 0; i < strlen(counter->me.content); i++)
                if(CCHAT_GLOBAL_SETTINGS & CCHAT_FLAG_NOCRYPTO)
                    fprintf(fd, "%c", counter->me.content[i]);
                else
                    fprintf(fd, "%c", ~counter->me.content[i]);
            fprintf(fd, "%c", 3);
            counter = counter->next;
        }
        fflush(fd);
        fclose(fd);
        if(fn_c == 0x0) free(fn);
    }
    region{ /* drop roles */
        if(fn_r == 0x0){//by default using roles.list
            fn = (char*)malloc(1024);
            sprintf(fn, "%s/%s", chat->syncer.path, "roles.list");
        } else fn = fn_r;
        FILE *fd = fopen(fn, "w");
        int max = getRolesCount(&chat->roler);
        struct Roler *roler_c = &chat->roler;
        for(int i = 0; i < max+1; i++){
            fprintf(fd, "%s\t", roler_c->role.name);//{", roler_c->role.name, roler_c->role.access);
            fwrite(&roler_c->role.access, sizeof(int), 1, fd);
            fprintf(fd, "\t{");
            //перечисление всех юзеров в роле
            struct UsersLinks *ulc = &roler_c->role.usersList;
            if(ulc->next != NULL){
                for(int ui = 0; ; ui++){
                    if(ulc == NULL) break;
                    if(ulc->id != -1)
                        fwrite(&ulc->id, 4, 1, fd);
                    ulc=ulc->next;
                }
            }
            fprintf(fd, "}");
            if(roler_c->next == NULL) break;
            else roler_c = roler_c->next;
        }
        fflush(fd);
        fclose(fd);
        if(fn_r == 0x0) free(fn);
    }
    return 0;
}

#endif