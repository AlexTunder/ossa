#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./types.h"

#define region      if(1)

//No crypto chat file
#define CCHAT_FLAG_NOCRYPTO 1<<0
//Drop only chat
#define CCHAT_FLAG_DROPOCWM 1<<1
//Enable native access mode
#define CCHAT_FLAG_ACCESS_E 1<<2
//Runtime network sending
#define CCHAT_FLAG_RUNTIMEN 1<<3
//Native audio support
#define CCHAT_FLAG_NAUDIO_E 1<<4

int CCHAT_GLOBAL_SETTINGS = CCHAT_FLAG_ACCESS_E;

struct FileSync initFileSync(const char *path, const char *cfgName){
    struct FileSync me;
    me.path = (char*)malloc(strlen(path));
    me.config_path = (char*)malloc(strlen(cfgName));
    strcpy(me.path, path);
    strcpy(me.config_path, cfgName);
    return me;
}
struct Message makeMes(const char *body, int userid){
    char *content;
    if(body != NULL){
        content = (char*)malloc(strlen(body));
        strcpy(content, body);
    }else content = NULL;
    struct Message mes = {content, time(NULL), userid};
    return mes;
}
struct UserList initUserList(){
    char *content = (char*)malloc(strlen("system")+1);
    strcpy(content, "system");
    #ifndef ENABLE_ACCESS
     struct UserList ul = {content, NULL};
    #else
     struct UserList ul = {content, 0b11, NULL};
    #endif
    return ul;
}
struct MessageList initML(){
    struct MessageList me = {makeMes("Chat opened", 0), NULL};
    return me;
}
struct UsersLinks UsersLinksinit(){
    struct UsersLinks me = {-1, NULL};
    return me;
}
struct Role initRole(){
    struct Role me;
    me.usersList = UsersLinksinit();
    me.name = (char*)malloc(strlen("member"));
    strcpy(me.name, "member");
    me.access = 0xffffffff;
    return me;
}
struct Roler initRoler(){
    struct Roler me = {initRole(), NULL};
    return me;
}

struct Chat initChat(const char *path){
    char *pth;
    if(path == NULL){
        pth = (char*)malloc(strlen(".")+1);
        strcpy(pth, ".");
    }else {
        pth = (char*)malloc(strlen(path)+1);
        strcpy(pth, path);
    }
    struct Chat me = {
    initFileSync(pth, "conf.cfg"),
    initUserList(),
    initML(), 
    initRoler()
    };
    return me;
}
struct Message getLastFromChat(struct Chat *chat){
    struct MessageList *counter = &chat->messages;
    for(; counter != NULL; counter = counter->next)
        if(counter->next == NULL) return counter->me;
    char err[1024];
    sprintf(err, "Error occuped while tring to find last message in %p MessageList. Reason - Counter start in NULL", (void*)&chat->messages);
    return makeMes(err, 0);
}
struct Message getLastFromML(struct MessageList *ml){
    struct MessageList *counter = ml;
    for(; counter != NULL; counter = counter->next)
        if(counter->next == NULL) return counter->me;
    char err[1024];
    sprintf(err,"Error occuped while tring to find last message in %p MessageList. Reason - Counter start in NULL", (void*)ml);
    return makeMes(err, 0);
}
struct Message getMessageML(struct MessageList *ml, int index){
    struct MessageList *counter = ml;
    for(int i = 0; counter != NULL; counter = counter->next, i++)
        if(counter->next == NULL || i == index) return counter->me;
    char err[1024];
    sprintf(err,"Error occuped while tring to find last message in %p MessageList. Reason - Counter start in NULL", (void*)ml);
    return makeMes(err, 0);
}
struct Message getMessage(struct Chat *chat, int index){
    return getMessageML(&chat->messages, index);
}
struct UsersLinks makeUsersLinks(int *links, int count){
    struct UsersLinks *counter = (struct UsersLinks*)malloc(sizeof(struct UsersLinks));
    struct UsersLinks me = {0, counter};
    if(links == NULL) return me;
    for(int i = 0; i < count; i++){
        counter->id = links[i];
        counter->next = (struct UsersLinks*)malloc(sizeof(struct UsersLinks));
        counter = counter->next;
    }
    return me;
}
struct Role *getRoleByIndex(struct Roler *roler, int index){
    struct Roler *counter = roler;
    for(int i = 0; i < index; i++)
        if(counter != NULL) counter = counter->next;
        else return NULL;
    return &counter->next->role;
}
struct UserList *getUserById(struct UserList *userList, int id){
    if(userList == NULL)
        return NULL;
    int i = 0;
    struct UserList *counter = userList;
    for(; i < id;i++){
        if(counter->next != NULL)
            counter = counter->next;
        else return NULL;
    }
    return counter;
}
struct MessageList loadMLFromFile(const char *filename){
    struct MessageList me = initML();
    #include "fbase.h"
    target = fopen(filename, "r");
    if(target == 0x0){
        printf("Failed to open file \'%s\' (reading MessageList)\n", filename);
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
            return me;
        }
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
struct UserList loadULFromFile(const char *filename){
    struct UserList me = initUserList();
    FILE *target = fopen(filename, "r");
    if(target == 0x0){
        printf("Failed to open file \'%s\' (reading MessageList)\n", filename);
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
struct Roler loadRolerFromFile(const char *filename){
    struct Roler me = initRoler();
    FILE *target = fopen(filename, "r");
    if(target == 0x0){
        printf("Failed to open file \'%s\' (reading MessageList)\n", filename);
        return me;
    }
    char c = 0, *name = (char*)malloc(32);
    while(1){

    }
    return me;
}

int getRolesCount(struct Roler *roler){
    struct Roler *counter = roler;
    int i = 0;
    for(;counter->next != NULL;i++){
        counter = counter->next;
    }
    return i;
}
char *getUsernameByID(int userid, struct UserList *userList){
    if(userList == NULL)
        return NULL;
    int i = 0;
    struct UserList *counter = userList;
    for(; i < userid;i++){
        if(counter->next != NULL)
            counter = counter->next;
        else return NULL;
    }
    return counter->name;
    // return getUserById(userList, userid)->name;
}
int pushUserToUL(const char *name, struct UserList *userList){
    if(userList == NULL) return -1; //error
    int index = 0;
    struct UserList *counter = userList;
    while(counter->next != NULL){
        index++;
        counter = counter->next;
    }
    counter->next = (struct UserList*)malloc(sizeof(struct UserList));
    counter->next->name = (char*)malloc(strlen(name));
    counter->next->next = NULL;
    strcpy(counter->next->name, name);
    return index;
}
int pushMessageToML(struct Message message, struct MessageList *ml){
    if(ml == NULL) return -1;
    int index = 0;
    struct MessageList *counter = ml;
    while(counter->next != NULL){
        index++;
        counter = counter->next;
    }
    counter->next = (struct MessageList *)malloc(sizeof(struct MessageList));
    counter->next->next = NULL;
    counter->next->me = message;
    return index;
}
int pushUser(const char *name, struct Chat *chat){
    return pushUserToUL(name, &chat->userList);
}
int pushMessage(struct Message message, struct Chat *chat){
    return pushMessageToML(message, &chat->messages);
}
int getMessagesCountML(struct MessageList *ml){
    struct MessageList *counter = ml; int index = 0;
    for(; counter != NULL; counter = counter->next, index++)
        if(counter->next == NULL) return index+1;
    char err[1024];
    sprintf(err,"Error occuped while tring to find last message in %p MessageList. Reason - Counter start in NULL", (void*)ml);
    return -1;
}
int getMessagesCount(struct Chat *chat){
    return getMessagesCountML(&chat->messages);
}
int getUsersCountUL(struct UserList *ml){
    struct UserList *counter = ml; int index = 0;
    for(; counter != NULL; counter = counter->next, index++)
        if(counter->next == NULL) return index+1;
    char err[1024];
    sprintf(err,"Error occuped while tring to find last user in %p UserList. Reason - Counter start in NULL", (void*)ml);
    return -1;
}
int getUsersCount(struct Chat *chat){
    return getUsersCountUL(&chat->userList);
}
void addRole(struct Roler *roler, const char *rolename, int access){
    struct Roler *counter = roler;
    int i = 0;
    for(; counter->next != NULL; i++){
        counter = counter->next;
    }
    counter->next = (struct Roler*)malloc(sizeof(struct Roler));
    counter->next->next = NULL;
    counter->next->role.name = (char*)malloc(strlen(rolename));
    strcpy(counter->next->role.name, rolename);
    counter->next->role.access = access;
    counter->next->role.usersList = UsersLinksinit();
    // printf("Using %p num, index - %i, %p->next = %p\n", counter, i, counter, counter->next);
}
void addUserToRole(struct Roler *roler, int id, int roleID, struct UserList* ul){
    int i = 0;
    struct Role *role = getRoleByIndex(roler, roleID);
    if(role == NULL){
        printf("fatal error: %i roleID is invalid (%p roler %i role index is %p)\n", roleID, roler,roleID,role);
    }else{
        i = 0;
        struct UsersLinks *counter = &role->usersList;
        if(counter == NULL){
            counter = (struct UsersLinks*)malloc(sizeof(struct UsersLinks));
            counter->id = id;
            counter->next = NULL;
        }else{
            for(; counter->next != NULL; i++){
                if(counter->id == id)
                    return;
                counter = counter->next;
            }
            counter->next = (struct UsersLinks*)malloc(sizeof(struct UsersLinks));
            counter = counter->next;
            counter->id = id;
            counter->next = NULL;
            #ifdef ENABLE_ACCESS
            if(ul != NULL){
                getUserById(ul, id)->access = role->access;
            }
            #endif
        }
    }
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
            fprintf(fd, "%s\t%x\t%i\t{", roler_c->role.name, roler_c->role.access, roler_c->role.access);
            //перечисление всех юзеров в роле
            struct UsersLinks *ulc = &roler_c->role.usersList;
            if(ulc->next != NULL){
                for(int ui = 0; ; ui++){
                    if(ulc == NULL) break;
                    if(ulc->id != -1){
                        fprintf(fd, " %i", ulc->id);
                    }
                    ulc=ulc->next;
                }
            }
            fprintf(fd, "}\n");
            if(roler_c->next == NULL) break;
            else roler_c = roler_c->next;
        }
        fflush(fd);
        fclose(fd);
        if(fn_r == 0x0) free(fn);
    }
    return 0;
}
int checkInRole(struct Role *role, int user){
    struct UsersLinks *counter = &role->usersList;
    for(int i = 0; counter->next != NULL; i++){
        if(counter->id == user) return 1; //User have permission
        counter = counter->next;
    }
    return 0; //User haven't permission
}

#ifdef ENABLE_ACCESS
int getUserAccess(int userid, struct UserList *userList){
    if(userList == NULL)
        return 0;
    int i = 0;
    struct UserList *counter = userList;
    for(; i < userid;i++){
        if(counter->next != NULL)
            counter = counter->next;
        else return -1;
    }
    return counter->access;
}
#endif
#ifdef stdChatEnable_Workspace
 #include "./workspace.c"
#endif