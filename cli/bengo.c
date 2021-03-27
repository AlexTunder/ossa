#include "./bengo.h"

struct __list_booster eset;

int main(int argc, char **argv){
    for(int i = 0; i < argc; i++){
        if(!strcmp(argv[i], "-uname")){
            conf.username = argv[++i];
        }else if(!strcmp(argv[i], "-conf")){
            conf._conf_path = argv[++i];
        }else if(!strcmp(argv[i], "-upr")){
            conf._u_pointrate = atoi(argv[++i]);
        }else if(!strcmp(argv[i], "-mpr")){
            conf._m_pointrate = atoi(argv[++i]);
        }else if(!strcmp(argv[i], "-ni")){
            conf._interface = BENGO_NC;
        }
    }

    if(conf.username == 0x0){
        cuserid(conf.username);
    }
    if(conf._conf_path == 0x0){
        conf._conf_path = (char*)malloc("./bengo.conf");
        strcpy(conf._conf_path, "./bengo.conf");
    }

    conf.conf = fopen(conf._conf_path, "r");
    if(!conf.conf){
        printf("["ANSI_COLOR_YELLOW"!!"ANSI_COLOR_RESET"] Failed to load config\n");
        if(conf._interface == 0)
            return -17;
    }
    /* setup scripts handle */

    /* interface handle */
    while(conf._interface == BENGO_CLI){

    }
}