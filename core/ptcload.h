#ifndef __WIN32
 #include <dlfcn.h>
#else
 #include "./dlfcn.h"
 #include "./dlfcn.c"
#endif
#ifndef TYPES_H
 #include "./types.h"
#endif
struct ProtoStorage{
    void *lib;

    struct LibStorage *next;

    int (*setServer)(const char *hostname, int port); //Make new connection to server
    int (*authServer)(const char *username, const char *pwd); //Auth in server. return your USERID
    int (*useServer)(int serverID); //User @serverID connection as main server
    int (*closeServer)(int serverID); //Stop server @serverID.
    int (*useraddServer)(const char *name, const char *pwd); //Try to make new account in server. return -1 if it's impossible or @userid

    /**              Sync API                **/
        /**          Messages              **/
    int (*sendMessage)(struct Message *message); //return count of new messages.
    int (*syncMessages)(struct MessageList *ml); //Sync messages with server. return count of diffs
    int (*delMessage)(struct Message *message); //Delete some message. return code
        /**  Roles, users and cmd   **/
    int (*syncRoles)(struct Roler *roler); //Sync roles with server. Return count of roles
    int (*syncUsers)(struct UserList *ul); //Sync users with server. Return count of users
    int (*pullRoles)(struct Roler *roler); //Try to pull local roles changes to server. Return code
    int (*pullUsers)(struct UserList *ul); //Try to pull local users changes to server. Return code
        /**            Files sync             **/
    int (*setRoot)(const char *path); //Set workspace root sync location
    int (*syncRoot)(void); //sync with server
    int (*loadRoot)(void); //Forse load all data from server. Removing all other data from root. returns count of loaded 
    int (*pullRoot)(void); //Try to replace server's chat-Root with local root. Returns allowed replaces.
};

struct ProtoStorage loadProto(const char *path);
int checkProto(struct ProtoStorage *proto);