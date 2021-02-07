#include "../src/languages.c"

void fail(const char *target, int reason){
    printf("Error in \'%s\', %i\n", target, reason);
}

int main(){
    strStorage.details.callback_fail = fail;
    loadLMFromFile("./languages/ru.lang", &strStorage);
    printf("%hs\n", strStorage.logic.yes);
}