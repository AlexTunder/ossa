#include "./neotype.h"
#include "./base.h"

#undef initUserList
#undef initML
#undef UsersLinksinit
#undef initRole
#undef getMLFromML /* idk, but this is useless JET */
#undef initChat

struct Message makeMes(const char *body, int userid){
    char *content;
    if(body != NULL){
        content = (char*)malloc(strlen(body));
        strcpy(content, body);
    }else content = NULL;
    struct Message mes = {content, time(0), userid};
    return mes;
}

struct Message getLastFromChat(struct Chat *chat){
    return getLastFromML(&chat->messages);
}
struct Message getLastFromML(struct MessageList* ml){
    struct Message mes;
    mes.date = astype(time_t) ml->end->data;
    mes.userid = *((int*)(ml->end->data)+sizeof(time_t));
    mes.content = (char*)((ml->end->data)+sizeof(time_t)+sizeof(int));
    return mes;
}
struct Message getMessageML(struct MessageList *ml, int index){
    struct Message mes;
    void *target = __booster_get(ml, index).target;
    mes.date = astype(time_t) target;
    mes.userid = *((int*)((target)+sizeof(time_t)));
    mes.content = (char*)((target)+sizeof(time_t)+sizeof(int));
    return mes;
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
    struct Role *me_ptr = new(struct Role);
    struct Role me;
    void *target = __booster_get(roler, index).target;
    me.access = astype(int) (target);
    me.membersCount = astype(int) ((target)+4);
    me.name = (char*) ((target)+8);
    makeUsersLinks((target)+8+strlen(me.name), me.membersCount);
    *me_ptr = me;
    return me_ptr;
}
