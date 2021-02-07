#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <wchar.h>
#include "../types.h"
#include "../base.c"
#include "../languages.c"

struct LangMap langmap;

void callback_load(const char *target){
    printf("[*]loaded: %s\n", target);
}
void callback_fail(const char *target, int code){
    printf("[!] failed with %i code in %s\n", code, target);
}

int main(){
    char *lcl = setlocale(LC_ALL, "");
    printf("Привет, cześć! Локаль: %s\n", lcl);
    fwide(stdout, 1);
    langmap.details.callback_load = callback_load;
    langmap.details.callback_fail = callback_fail;
    int loaded  = loadLMFromFile("../../languages/pl.lang", &langmap);
    printf("loaded data: %i\n", loaded);
    setlocale(LC_ALL, (char*)langmap.details.locale);
    printf("langmap.details.locale: \'%s\'\n", langmap.details.locale);
    printf("langmap.logic.yes: \'%s\'\n", (char*)langmap.logic.yes);
    printf("langmap.logic.no: \'%s\'\n", (char*)langmap.logic.no);
}