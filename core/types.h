#pragma once
#include <time.h>
#ifndef TYPES_H
#define TYPES_H
// basic
struct UserList{
    char *name;
    #ifdef ENABLE_ACCESS
     int access;
    #endif
    struct UserList *next;
};
struct Message{
    char *content; time_t date; int userid;
};
struct MessageList{
    struct Message me;
    struct MessageList *next;
};
struct FileSync{
    char *path;
    char *config_path;
};
//Roles
struct UsersLinks{
    int id;
    struct UsersLinks *next;
};
struct Role{
    struct UsersLinks usersList;
    char *name;
    int access;
};
struct Roler{
    struct Role role;
    struct Roler *next;
};
//Basic Chat
struct Chat{
    struct FileSync syncer;
    struct UserList userList;
    struct MessageList messages;
    struct Roler roler;
};
#endif