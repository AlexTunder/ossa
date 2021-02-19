#include <ncurses.h>			/* ncurses.h includes stdio.h */  
#include <string.h> 

char types[1024*3];
int row,col;
 
 void render_textField(){
     
 }
 void render_typeField(){
     mvprintw(row-1, 0, "user: ");
     printw(types);
 }

void render(){
    render_textField();
    render_typeField();
}

int main(){
    initscr();
    echo();
    raw();
    int gc = 0;
    memset(types, 0, 1024*3);
    do{
        clear();
        getmaxyx(stdscr,row,col);
        render();
        gc = getch();
        int i = 0;
        for(; types[i] != 0; i++);
        types[i] = gc;
        refresh();
    }while(gc != 27);
    endwin();
 return 0;
}