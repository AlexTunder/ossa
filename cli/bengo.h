#include "../core/types.h"
#include "../core/list.c"

#include <stdio.h>
#include <string.h>

//CLI codes
#define CLI_OK_NOUT 0x00f0
#define CLI_OK_DROP 0x00f1
#define CLI_EXIT    0x06fc
#define CLI_ERRF    0x0600
#define CLI_CNF     0x06ff
#define CLI_BOS     0x06ac
#define CLI_NOPERM  0x0700
//Access codes
#define acc_read 1<<0  /* read messages */
#define acc_write 1<<1 /* send messages */
#define acc_users 1<<2 /* manage users */
#define acc_roler 1<<3 /* manage rolies */
#define acc_taggi 1<<4 /* tag anyone */
#define acc_evlog 1<<5 /* login as everyone */
//COLORS
#ifndef ANSI_COLOR_RED 
    #define ANSI_COLOR_RED     "\x1b[31m"
    #define ANSI_COLOR_GREEN   "\x1b[32m"
    #define ANSI_COLOR_YELLOW  "\x1b[33m"
    #define ANSI_COLOR_BLUE    "\x1b[34m"
    #define ANSI_COLOR_MAGENTA "\x1b[35m"
    #define ANSI_COLOR_CYAN    "\x1b[36m"
    #define ANSI_COLOR_RESET   "\x1b[0m"
#endif

#define BENGO_NC    0
#define BENGO_CLI   1
#define BENGO_GUI   2

struct _config{
    char *_exec_path, *_conf_path;
    char *username;
    int _u_pointrate, _m_pointrate;
    char _interface;
    FILE *conf;
}conf = {0x0, 0x0, 0x0, 16, 32, BENGO_CLI};

void getLocationOfExe(char *to){
#ifdef __WIN32
    GetModuleFileNameW(NULL, to, MAX_PATH);
#else

#endif
}

int handleScript(const char *script){
    
}
/** let User - is 4 first bytes of acces and next - string of name **/