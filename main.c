#include <ncurses.h>

typedef struct{
    int xpos;
    int ypos;
    int hp;
} Player;

int screenSet();
int mapSet();
void *playerSet(Player **player);

int main(){
    Player *player;
    playerSet(&player);


    screenSet();
    mapSet();
    getch();

    endwin();
    return 0;
}

int mapSet(){
    
}

int screenSet(){
    initscr();
    printw("Hello world!");
    noecho();
    refresh();

    return 0;
}

// Sets up pointer to a pointer?
void *playerSet(Player **player){
    Player *player = malloc(sizeof(Player));
    (*player)->xpos = 0;
    (*player)->ypos = 0;
    (*player)->hp = 100;

    mvprintw((*player)->ypos, (*player)->xpos, "@");
}