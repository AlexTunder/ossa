#pragma once

#ifndef BASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef TYPES_H
 #include "./types.h"
#endif

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

/**
 * @brief Make new message
 * 
 * @param body - Body of message
 * @param userid - Sender UID
 * @return struct Message 
 */
struct Message makeMes(const char *body, int userid);
/**
 * @brief Init empty UserList
 * 
 * @return struct UserList 
 */
struct UserList initUserList();
/**
 * @brief Init empty Message List
 * 
 * @return struct MessageList 
 */
struct MessageList initML();
/**
 * @brief Init empty UserLinks
 * 
 * @return struct UsersLinks 
 */
struct UsersLinks UsersLinksinit();
/**
 * @brief Init empty role
 * 
 * @return struct Role 
 */
struct Role initRole();
/**
 * @brief Init empty roler
 * 
 * @return struct Roler 
 */
struct Roler initRoler();
/**
 * @brief Init empty chat
 * 
 * @param path 
 * @return struct Chat 
 */
struct Chat initChat(const char *path);
/**
 * @brief Get the Last From Chat object
 * 
 * @param chat - target
 * @return struct Message 
 */
struct Message getLastFromChat(struct Chat *chat);
/**
 * @brief Get the Last From MessageList object
 * 
 * @param ml - MessageList
 * @return struct Message 
 */
struct Message getLastFromML(struct MessageList *ml);
/**
 * @brief Get the Message from MessageList
 * 
 * @param ml - Source message list
 * @param index - index of target
 * @return struct Message 
 */
struct Message getMessageML(struct MessageList *ml, int index);
/**
 * @brief LOL what?
 * 
 * @param ml 
 * @param index 
 * @return struct MessageList* 
 */
struct MessageList *getMLFromML(struct MessageList *ml, int index);
/**
 * @brief Get the Message object from chat
 * 
 * @param chat - source chat
 * @param index - index of message
 * @return struct Message 
 */
struct Message getMessage(struct Chat *chat, int index);
/**
 * @brief Make Userlist from int array
 * 
 * @param links - array of UIDs
 * @param count - len of array
 * @return struct UsersLinks 
 */
struct UsersLinks makeUsersLinks(int *links, int count);
/**
 * @brief Get the Role By Index from Roler
 * 
 * @param roler - Source roler. Role storage
 * @param index - index of role
 * @return struct Role* 
 */
struct Role *getRoleByIndex(struct Roler *roler, int index);
/**
 * @brief Get the User By Id from UserList
 * 
 * @param userList - source list of users
 * @param id - user ID (UID)
 * @return struct UserList* 
 */
struct UserList *getUserById(struct UserList *userList, int id);

/**
 * @brief Get the Roles Count
 * 
 * @param roler - roles source
 * @return int count
 */
int getRolesCount(struct Roler *roler);
/**
 * @brief Get the Username By ID from UserList
 * 
 * @param userid - UID
 * @param userList - source userlist
 * @return char* username in C-like string
 */
char *getUsernameByID(int userid, struct UserList *userList);
/**
 * @brief push new user to UserList. Same as add new user
 *  Low level. Do not touch!
 * @param name - Username
 * @param userList - source user list 
 * @return int index of new user (UID)
 */
int pushUserToUL(const char *name, struct UserList *userList);
/**
 * @brief Add new message to MessageList
 *  Low level. Do not touch!
 * @param message - message for append
 * @param ml - targeted message list
 * @return int - index of new item
 */
int pushMessageToML(struct Message message, struct MessageList *ml);
/**
 * @brief Add new user to chat
 * 
 * @param name - username
 * @param chat - target for append
 * @return int - UID of new user
 */
int pushUser(const char *name, struct Chat *chat);
/**
 * @brief Add new message to chat
 * 
 * @param message - message
 * @param chat - target for append
 * @return int - message index in list
 */
int pushMessage(struct Message message, struct Chat *chat);
/**
 * @brief Get the Messages Count from MessageList
 *  Low level. Do not touch!
 * @param ml - MessageList source
 * @return int - count
 */
int getMessagesCountML(struct MessageList *ml);
/**
 * @brief Get the Messages Count from chat
 * 
 * @param chat - source
 * @return int - count
 */
int getMessagesCount(struct Chat *chat);
/**
 * @brief Get the Users Count from UserList
 *  Low level. Do not touch!
 * @param ml - source UserList
 * @return int - count
 */
int getUsersCountUL(struct UserList *ml);
/**
 * @brief Get the Users Count from chat
 * 
 * @param chat - source
 * @return int - count
 */
int getUsersCount(struct Chat *chat);
/**
 * @brief Add new role to roler
 *
 * @param roler - roles strorage
 * @param rolename - name of role
 * @param access - access 32-bit-mask
 */
void addRole(struct Roler *roler, const char *rolename, int access);
/**
 * @brief add new user to role
 * 
 * @param roler - source roles storage
 * @param id - UID for role's member
 * @param roleID - role index
 * @param ul - source of UserList. Where strores targeted user
 */
void addUserToRole(struct Roler *roler, int id, int roleID, struct UserList* ul);
/**
 * @brief Check user for role
 * 
 * @param role - Role
 * @param user - user
 * @return int  - true/false
 */
int checkInRole(struct Role *role, int user);
/**
 * @brief Get the User Access
 * 
 * @param userid - UID of target
 * @param userList - List, stores UID
 * @return int - 32-bit-mask of access
 */
int getUserAccess(int userid, struct UserList *userList);

 #ifdef CCHAT_USE_SRC
  #include "./base.c"
 #else
  /// Я потом вставлю сюда объявления функций

 #endif
#endif
