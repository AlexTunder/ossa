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
 #ifdef CCHAT_USE_SRC
  #include "./base.c"
 #else
  /// Я потом вставлю сюда объявления функций

 #endif
#endif
