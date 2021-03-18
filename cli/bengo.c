#include "../core/types.h"
#include "../core/list.c"

#include <stdio.h>

/** let User - is 4 first bytes of acces and next - string of name **/

struct __list_booster users;

int main(int argc, char **argv){
    if(argc < 2){
        printf("You should to use \"./bengo uname\"\n");
        return -1;
    }
    users = makeEmptyBooster(16); //16 navigate points
    int *ptr = (int*)__booster_append(&users, 4+strlen(argv[1])).target;
    *ptr = 1024;
    ptr = &ptr[1];
    char *name = (char*)ptr;
    strcpy(name, argv[1]);
    ptr = (int*)__booster_append(&users, 4+strlen("argv[1]")).target;
    *ptr = 1224;
    ptr = &ptr[1];
    name = (char*)ptr;
    strcpy(name, "argv[1]");
    printf("Full, unsafe: %s\n\taccess: %i\n\tname:%s\n", __booster_get(&users, 1).target, astype(int)__booster_get(&users, 1).target,(char*)__booster_get(&users, 1).target+4);
}