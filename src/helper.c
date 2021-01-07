#include "types.h"
#include "chat.c"
#include <stdio.h>
struct MessageList loadMLFromStream(const char *target){
    #include "fbase.h"
    struct MessageList me = initML();
    if(target == 0x0){
        printf("Failed to open stream (reading MessageList)\n");
        return me;
    }
    char c = 0;
    char err[1024];
    while(1){ /*read one message*/
        if(fread(&c, 1, 1, target) == 0) break;
        //Syntax check
        if(c != 1){
            sprintf(err, "Failed to read chat from stream: corrupted syntax (no HEAD)");
            pushMessageToML(makeMes(err, 0), &me);
            return me;
        }
        struct Message sub = makeMes("", 0);
        fread(&sub.userid, sizeof(sub.userid), 1, target);
        fread(&sub.date, sizeof(sub.date), 1, target);
        fread(&c, 1, 1, target);
        //Syntax check
        if(c!=2){
            sprintf(err, "Failed to read chat from stream: corrupted syntax (no BODY-START)");
            pushMessageToML(makeMes(err, 0), &me);
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
    free(mesBuf);
    return me;
}
struct UserList loadULFromStream(const char *target){
    struct UserList me = initUserList();
    if(target == 0x0){
        printf("Failed to open stream cuz it's NULL (reading MessageList)\n");
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
    return me;
}
struct Roler loadRolerFromStream(const char *target){
    struct Roler me = initRoler();
    if(target == 0x0){
        fprintf(stderr, "Failed to open stream cuz it's NULL (reading Roler)\n");
        return me;
    }
    char c = 0, *name = (char*)malloc(32);
    for(long roleid = 0; ; roleid++){
        struct Role this;
        // if(fread(&c, 1, 1, target) == 0) return me;
        // else fseek(target, SEEK_CUR-1, 0);
        for(int i = 0; c != '\t'; i++){
            if(fread(&c, 1, 1, target) == 0) return me;
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
        fread(&this.access, 4, 1, target);
        fread(&c, 1, 1, target);
        if(c != '\t'){
            fprintf(stderr, "Fatal error: invelid syntax in %p stream. \' \' is skipped, stage 1. unexpected: \'%c\'(%i)\n", target, c, c);
            return initRoler();
        }
        fread(&c, 1, 1, target);
        if(c != '{'){
            fprintf(stderr, "Fatal error: invelid syntax in %p stream. \'{\' is skipped, stage 3. unexpected: \'%c\'(%i)\n", target, c, c);
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
    return me;
}
