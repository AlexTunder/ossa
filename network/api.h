#pragma once
#include "../src/types.h"

#define OSSA_SERVER_CONNECTON   -1
#define OSSA_FAILED_GET_HARDWR  -2
#define OSSA_NET_INVALID_PROTO   -3
#define OSSA_AUTH_FAILED                 -4

/**         Connection API          **/
extern int setServer(const char *hostname, int port); //Make new connection to server
extern int authServer(const char *username, const char *pwd); //Auth in server. return your USERID
extern int useServer(int serverID); //User @serverID connection as main server
extern int closeServer(int serverID); //Stop server @serverID.
extern int useraddServer(const char *name, const char *pwd); //Try to make new account in server. return -1 if it's impossible or @userid

/**              Sync API                **/
    /**          Messages              **/
extern int sendMessage(struct Message *message); //return count of new messages.
extern int syncMessages(struct MessageList *ml); //Sync messages with server. return count of diffs
extern int delMessage(struct Message *message); //Delete some message. return code
    /**  Roles, users and cmd   **/
extern int syncRoles(struct Roler *roler); //Sync roles with server. Return count of roles
extern int syncUsers(struct UserList *ul); //Sync users with server. Return count of users
extern int pullRoles(struct Roler *roler); //Try to pull local roles changes to server. Return code
extern int pullUsers(struct UserList *ul); //Try to pull local users changes to server. Return code
    /**            Files sync             **/
extern int setRoot(const char *path); //Set workspace root sync location
extern int syncRoot(); //sync with server
extern int loadRoot(); //Forse load all data from server. Removing all other data from root. returns count of loaded 
extern int pullRoot(); //Try to replace server's chat-Root with local root. Returns allowed replaces.