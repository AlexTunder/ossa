// #pragma once
#ifndef CCHAT_BASE_H
 #include "base.h"
#endif

#ifndef CCHAT_BASE_C
#define CCHAT_BASE_C

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
    struct Message mes = {content, time(0), userid};
    return mes;
}
struct UserList initUserList(){
    char *content = (char*)malloc(strlen("system")+1);
    strcpy(content, "system");
    #ifdef DISABLE_ACCESS
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
struct MessageList *getMLFromML(struct MessageList *ml, int index){
    struct MessageList *counter = ml;
    for(int i = 0; counter != NULL; counter = counter->next, i++)
        if(counter->next == NULL || i == index) return counter;
    char err[1024];
    sprintf(err,"Error occuped while tring to find last message in %p MessageList. Reason - Counter start in NULL", (void*)ml);
    return 0x0;
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
    if(counter == 0x0) return 0x0;
    if(index == 0) return &roler->role;
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
        fprintf(stderr, "\x1b[31m[FATAL ERROR]\x1b[0m: %i roleID is invalid (%p roler %i role index is %p)\n", roleID, roler,roleID,role);
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
            #ifndef DISABLE_ACCESS
            if(ul != NULL){
                getUserById(ul, id)->access = role->access;
            }
            #endif
        }
    }
}
int checkInRole(struct Role *role, int user){
    struct UsersLinks *counter = &role->usersList;
    for(int i = 0; counter->next != NULL; i++){
        if(counter->id == user) return 1; //User have permission
        counter = counter->next;
    }
    return 0; //User haven't permission
}
struct ChatList *getChatChainByIndex(struct ChatList *root, int index){
    if(index == 0)
        return root;
    else return getChatChainByIndex(root->next, index-1);
}
int getChatChainLen(struct ChatList *root){
    if(root->next == 0x0)
        return 1;
    else return getChatChainLen(root->next)+1;
}
int pushChatToCL(struct ChatList *root, struct Chat chat){
    if(root->next == 0x0){
        root->next = (struct ChatList*) malloc(sizeof(struct ChatList));
        root->next->chat = chat;
        root->next->next = 0x0;
        #ifdef OSSA_ASYNC
        root->next->serverID = -1;
        #endif
        return 1;
    }else return pushChatToCL(root->next, chat)+1;
}
#ifndef DISABLE_ACCESS
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
#ifdef OSSA_ASYNC
struct ChatList *getChatFromServerID(struct ChatList *root, int index){
    if(root->serverID == index) return root;
    else if(root->next == 0x0) return 0x0;
    else return getChatFromServerID(root->next, index);
}
#endif
#ifdef stdChatEnable_Workspace
 #include "./workspace.c"
#endif
#endif