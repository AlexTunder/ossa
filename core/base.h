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
//Enable native soft-access mode
#define CCHAT_FLAG_ACCESS_E 1<<2
//Runtime network sending
#define CCHAT_FLAG_RUNTIMEN 1<<3
//Native audio support
#define CCHAT_FLAG_NAUDIO_E 1<<4
//Dynamic or static language map
#define CCHAT_FLAG_DYNLANMP 1<<5

#ifdef SETTINGS
int CCHAT_GLOBAL_SETTINGS = CCHAT_FLAG_ACCESS_E | CCHAT_FLAG_DYNLANMP;
#endif

/**
 * @brief Make new message
 * 
 * @param body - Body of message
 * @param userid - Sender UID
 * @return struct Message 
 */ extern
struct Message makeMes(const char *body, int userid);
/**
 * @brief Init empty UserList
 * 
 * @return struct UserList 
 */ extern
struct UserList initUserList();
/**
 * @brief Init empty Message List
 * 
 * @return struct MessageList 
 */ extern
struct MessageList initML();
/**
 * @brief Init empty UserLinks
 * 
 * @return struct UsersLinks 
 */ extern
struct UsersLinks UsersLinksinit();
/**
 * @brief Init empty role
 * 
 * @return struct Role 
 */ extern
struct Role initRole();
/**
 * @brief Init empty roler
 * 
 * @return struct Roler 
 */ extern
struct Roler initRoler();
/**
 * @brief Init empty chat
 * 
 * @param path 
 * @return struct Chat 
 */ extern
struct Chat initChat(const char *path);
/**
 * @brief Get the Last From Chat object
 * 
 * @param chat - target
 * @return struct Message 
 */ extern
struct Message getLastFromChat(struct Chat *chat);
/**
 * @brief Get the Last From MessageList object
 * 
 * @param ml - MessageList
 * @return struct Message 
 */ extern
struct Message getLastFromML(struct MessageList *ml);
/**
 * @brief Get the Message from MessageList
 * 
 * @param ml - Source message list
 * @param index - index of target
 * @return struct Message 
 */ extern
struct Message getMessageML(struct MessageList *ml, int index);
/**
 * @brief LOL what?
 * 
 * @param ml 
 * @param index 
 * @return struct MessageList* 
 */ extern
struct MessageList *getMLFromML(struct MessageList *ml, int index);
/**
 * @brief Get the Message object from chat
 * 
 * @param chat - source chat
 * @param index - index of message
 * @return struct Message 
 */ extern
struct Message getMessage(struct Chat *chat, int index);
/**
 * @brief Make Userlist from int array
 * 
 * @param links - array of UIDs
 * @param count - len of array
 * @return struct UsersLinks 
 */ extern
struct UsersLinks makeUsersLinks(int *links, int count);
/**
 * @brief Get the Role By Index from Roler
 * 
 * @param roler - Source roler. Role storage
 * @param index - index of role
 * @return struct Role* 
 */ extern
struct Role *getRoleByIndex(struct Roler *roler, int index);
/**
 * @brief Get the User By Id from UserList
 * 
 * @param userList - source list of users
 * @param id - user ID (UID)
 * @return struct UserList* 
 */ extern
struct UserList *getUserById(struct UserList *userList, int id);

/**
 * @brief Get the Roles Count
 * 
 * @param roler - roles source
 * @return int count
 */ extern
int getRolesCount(struct Roler *roler);
/**
 * @brief Get the Username By ID from UserList
 * 
 * @param userid - UID
 * @param userList - source userlist
 * @return char* username in C-like string
 */ extern
char *getUsernameByID(int userid, struct UserList *userList);
/**
 * @brief push new user to UserList. Same as add new user
 *  Low level. Do not touch!
 * @param name - Username
 * @param userList - source user list 
 * @return int index of new user (UID)
 */ extern
int pushUserToUL(const char *name, struct UserList *userList);
/**
 * @brief Add new message to MessageList
 *  Low level. Do not touch!
 * @param message - message for append
 * @param ml - targeted message list
 * @return int - index of new item
 */ extern
int pushMessageToML(struct Message message, struct MessageList *ml);
/**
 * @brief Add new user to chat
 * 
 * @param name - username
 * @param chat - target for append
 * @return int - UID of new user
 */ extern
int pushUser(const char *name, struct Chat *chat);
/**
 * @brief Add new message to chat
 * 
 * @param message - message
 * @param chat - target for append
 * @return int - message index in list
 */ extern
int pushMessage(struct Message message, struct Chat *chat);
/**
 * @brief Get the Messages Count from MessageList
 *  Low level. Do not touch!
 * @param ml - MessageList source
 * @return int - count
 */ extern
int getMessagesCountML(struct MessageList *ml);
/**
 * @brief Get the Messages Count from chat
 * 
 * @param chat - source
 * @return int - count
 */ extern
int getMessagesCount(struct Chat *chat);
/**
 * @brief Get the Users Count from UserList
 *  Low level. Do not touch!
 * @param ml - source UserList
 * @return int - count
 */ extern
int getUsersCountUL(struct UserList *ml);
/**
 * @brief Get the Users Count from chat
 * 
 * @param chat - source
 * @return int - count
 */ extern
int getUsersCount(struct Chat *chat);
/**
 * @brief Add new role to roler
 *
 * @param roler - roles strorage
 * @param rolename - name of role
 * @param access - access 32-bit-mask
 */ extern
void addRole(struct Roler *roler, const char *rolename, int access);
/**
 * @brief add new user to role
 * 
 * @param roler - source roles storage
 * @param id - UID for role's member
 * @param roleID - role index
 * @param ul - source of UserList. Where strores targeted user
 */ extern
void addUserToRole(struct Roler *roler, int id, int roleID, struct UserList* ul);
/**
 * @brief Check user for role
 * 
 * @param role - Role
 * @param user - user
 * @return int  - true/false
 */ extern
int checkInRole(struct Role *role, int user);
/**
 * @brief Get the User Access
 * 
 * @param userid - UID of target
 * @param userList - List, stores UID
 * @return int - 32-bit-mask of access
 */ extern
int getUserAccess(int userid, struct UserList *userList);
/**
 * @brief Destroy all struct of userlist
 * 
 * @param ul - UserList target
 * @return int - count of deleted data  
 */
int destroyUL(struct UserList *ul);
/**
 * @brief Destroing all elements of message list
 * 
 * @param ml - target
 * @return int - count of deleted data
 */
int destroyML(struct MessageList *ml);

struct ChatList *getChatChainByIndex(struct ChatList *root, int index);
int getChatChainLen(struct ChatList *root);
int pushChatToCL(struct ChatList *root, struct Chat chat);
#ifdef OSSA_ASYNC
struct ChatList *getChatFromServerID(struct ChatList *root, int index);
#endif
 #ifdef CCHAT_USE_SRC
  #include "./base.c"
 #endif
#endif
