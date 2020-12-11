#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CLI_EXIT    0x06fc
#define CLI_ERRF    0x0600
#define CLI_CNF     0x06ff
#define CLI_BOS     0x06ac

struct UserList{
    char *name;
    struct UserList *next;
};
struct Message{
    char *content; time_t date; int userid;
};
struct MessageList{
    struct Message me;
    struct MessageList *next;
};
struct Chat{
    struct UserList userList;
    struct MessageList messages;
};

struct Message makeMes(const char *body, int userid){
    char *content;
    if(body != NULL){
        content = (char*)malloc(strlen(body));
        strcpy(content, body);
    }else content = NULL;
    struct Message mes = {content, time(NULL), userid};
    return mes;
}
struct UserList initUserList(){
    char *content = (char*)malloc(strlen("system"));
    strcpy(content, "system");
    struct UserList ul = {content, NULL};
    return ul;
}
struct MessageList initML(){
    struct MessageList me = {makeMes("Chat opened", 0), NULL};
    return me;
}
struct Chat initChat(){
    struct Chat me = {initUserList(), initML()};
    return me;
}
struct Message getLastFromChat(struct Chat *chat){
    struct MessageList *counter = &chat->messages;
    for(; counter != NULL; counter = counter->next)
        if(counter->next == NULL) return counter->me;
    char err[1024];
    sprintf(err, "Error occuped while tring to find last message in %p MessageList. Reason - Counter start in NULL", (void*)&chat->messages);
    return makeMes(err, 0);
}
struct Message getLastFromML(struct MessageList *ml){
    struct MessageList *counter = ml;
    for(; counter != NULL; counter = counter->next)
        if(counter->next == NULL) return counter->me;
    char err[1024];
    sprintf(err,"Error occuped while tring to find last message in %p MessageList. Reason - Counter start in NULL", (void*)ml);
    return makeMes(err, 0);
}
struct Message getMessageML(struct MessageList *ml, int index){
    struct MessageList *counter = ml;
    for(int i = 0; counter != NULL; counter = counter->next, i++)
        if(counter->next == NULL || i == index) return counter->me;
    char err[1024];
    sprintf(err,"Error occuped while tring to find last message in %p MessageList. Reason - Counter start in NULL", (void*)ml);
    return makeMes(err, 0);
}
struct Message getMessage(struct Chat *chat, int index){
    return getMessageML(&chat->messages, index);
}

char *getUsernameByID(int userid, struct UserList *userList){
    if(userList == NULL)
        return NULL;
    int i = 0;
    struct UserList *counter = userList;
    for(; i < userid;i++){
        if(counter->next != NULL)
            counter = counter->next;
        else return NULL;
    }
    return counter->name;
}
int pushUserToUL(const char *name, struct UserList *userList){
    if(userList == NULL) return -1; //error
    int index = 0;
    struct UserList *counter = userList;
    while(counter->next != NULL){
        index++;
        counter = counter->next;
    }
    counter->next = (struct UserList*)malloc(sizeof(struct UserList));
    counter->next->name = (char*)malloc(strlen(name));
    counter->next->next = NULL;
    strcpy(counter->next->name, name);
    return index;
}
int pushMessageToML(struct Message message, struct MessageList *ml){
    if(ml == NULL) return -1;
    int index = 0;
    struct MessageList *counter = ml;
    while(counter->next != NULL){
        index++;
        counter = counter->next;
    }
    counter->next = (struct MessageList *)malloc(sizeof(struct MessageList));
    counter->next->next = NULL;
    counter->next->me = message;
    return index;
}
int pushUser(const char *name, struct Chat *chat){
    pushUserToUL(name, &chat->userList);
}
int pushMessage(struct Message message, struct Chat *chat){
    pushMessageToML(message, &chat->messages);
}
int getMessagesCountML(struct MessageList *ml){
    struct MessageList *counter = ml; int index = 0;
    for(; counter != NULL; counter = counter->next, index++)
        if(counter->next == NULL) return index+1;
    char err[1024];
    sprintf(err,"Error occuped while tring to find last message in %p MessageList. Reason - Counter start in NULL", (void*)ml);
    return -1;
}
int getMessagesCount(struct Chat *chat){
    return getMessagesCountML(&chat->messages);
}

//UI

int handleCommand(char comm[16][32]){
    if(!strcmp(comm[0], "exit"))
        return CLI_EXIT;
    else return CLI_CNF;
}
int handleCLI(struct Chat *chat, int *me){
    char y = getchar();
    if(y == ':'){
        //Handle command
        char comms[16][32];
        char c = 0;
        int commcounter = 0;
        for( ; ; ){
            for(int j = 0; j < 32 && c != '\n' && c != ' ' ; j++){
                c = getchar();
                if(c!='\n' && c!=8) comms[commcounter][j] = c;
                if(c==8) j-=2;
            }
            if(c == '\n') break;
            ++commcounter;
        }
        return handleCommand(comms);
    }else if(y == '$'){
        //Handle system command
    }else{
        //Handle new message
        char c = 0;
        char input[1024];
        int counter = 0;
        input[0] = y;
        do{
            c = getchar();
            if(c == '\n') break;
            else if(c == 8) counter -= 2;
            else input[++counter] = c;
        }while(c != '\n');
        pushMessage(makeMes(input, *me), chat);
    }
    return 0;
} 

int main(){
    /** Test example
    struct UserList ul = initUserList();
    pushUserToUL("alex", &ul);
    struct MessageList ml = initML();
    pushMessageToML(makeMes("Hello!", 1), &ml);
    pushMessageToML(makeMes("Hello!", 0), &ml);
    struct Message mes = getLastFromML(&ml);
    printf("%s@[system]: %s\n <%i>\n", getUsernameByID(mes.userid, &ul), mes.content, mes.date);
    */

   int me = 1;
   struct Chat chat = initChat();
   pushUser("alex", &chat); //1st user
   pushUser("andrezoo", &chat); //2nd user
   int last = 0;
   pushMessage(makeMes("Hello, Andrey!", 1), &chat);
   pushMessage(makeMes("Hello, Sanya!", 2), &chat);
   while(1){
    for(int i = last; i < getMessagesCount(&chat); i++){
            struct Message mes = getMessage(&chat, i); 
            printf("(%i)%s@[system]: %s\n sended:<%i>\n", i, getUsernameByID(mes.userid, &chat.userList), mes.content, mes.date);
            last = i+1;
    }
    printf("%s@[sys]: ", getUsernameByID(me, &chat.userList));
    int code = handleCLI(&chat, &me);
    printf("%s@[system]: %s\n <%i>\n", getUsernameByID(getLastFromChat(&chat).userid, &chat.userList), getLastFromChat(&chat).content, getLastFromChat(&chat).date);
    if(code == 0) last ++;
    else if(code >= CLI_ERRF || code == CLI_EXIT){
        printf("Exit...\n");
        break;
    }
   }
   return 0;
}