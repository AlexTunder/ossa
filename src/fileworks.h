#pragma once
#ifndef CCHAT_TYPES_H
 #include "types.h"
#endif
#ifndef CCHAT_BASE_H
#include "base.h"
#endif
#ifndef CCHAT_USE_SRC
 #ifndef CCHAT_FILEWORKS_H
  struct MessageList loadMLFromStream(const char *target);
  struct UserList loadULFromStream(const char *target);
  struct Roler loadRolerFromStream(const char *target);
  struct MessageList loadMLFromFile(const char *filename);
  struct UserList loadULFromFile(const char *filename);
  struct Roler loadRolerFromFile(const char *filename);
  int dropChatToFile(struct Chat *chat, char *fn_c, char *fn_u, char *fn_r);
#endif
 #else
 #ifndef CCHAT_FILEWORKS_C
  #include "./fileworks.c"
 #endif
#endif