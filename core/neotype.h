#pragma once
#include "./list.h"
#ifndef TYPES_H
#define TYPES_H
// basic
#define UserList __list_booster
#define MessageList __list_booster
#define Roler __list_booster
#define ChatList __list_booster
struct Message{
    char *content; time_t date; int userid;
};
//Roles
struct UsersLinks{
    int id;
    struct UsersLinks *next;
};
struct Role{
    struct UsersLinks usersList;
    char *name;
    int access, membersCount;
};
//Basic Chat
struct Chat{
    struct UserList userList;
    struct MessageList messages;
    struct Roler roler;
};
#endif