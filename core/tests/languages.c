#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <wchar.h>
#include "../types.h"
#include "../base.c"
#include "../envl.c"

struct DynamicLanguageMap dlm = {0x0, 0x0};

void callback_load(const char *target){
    printf("[*]loaded: %s\n", target);
}
void callback_fail(const char *target, int code){
    printf("[!] failed with %i code in %s\n", code, target);
}

int main(){
    char *lcl = setlocale(LC_ALL, ".utf8");
    // printf("Привет, cześć! Локаль: %s\n", lcl);
    // fwide(stdout, 1);
    int loaded  = loadDLMFromFile("../../languages/pl.lang", &dlm);
    printf("loaded vars: %i\n", loaded);
    printf("dlm.logic.yes: \'%s\'\n", dlmGetTranslation(&dlm, "yes"));
    for(int i = 0; i < loaded; i++){
        printf("\'%s\' translation: \'%s\'\n", dlmGetTranslationByIndex(&dlm, i)->fieldname, dlmGetTranslationByIndex(&dlm, i)->translation);
    }
}