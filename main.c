#include <ncurses.h>
#include <stdlib.h>

typedef struct{

} Monster;

typedef struct{

} Item;

typedef struct{
    int xpos;
    int ypos;
    int height;
    int width;
    Monster *monsters;
    Item *items;
} Room;

typedef struct{
    int xpos;
    int ypos;
    int hp;
} Player;

int screenSet();
Room ** mapSet();
Room * createRoom(int x, int y, int height, int width);
int drawRoom(Room *room);
int overlap(Room *room1, Room *room2);
int handleInput(int input, Player *player);
int playerMove(int ypos, int xpos, Player *player);
int checkPosition(int x, int y);
void *playerSet(Player **player);

int main(){
    Player *player;
    int input; // int works because getch() returns an int and chars promote to int anyways
    
    screenSet();
    mapSet();
    playerSet(&player);

    // main game loop
    while((input = getch()) != 'q'){
        handleInput(input, player);
    }

    endwin();
    return 0;
}

Room ** mapSet(){
    // procedurally generate map

    // random no. of rooms 
    int roomCount = rand() % 5 + 3;
    Room **rooms = malloc(sizeof(Room*) * roomCount);

    int created = 0;
    while (created < roomCount) {
        int height = rand() % 5 + 3;
        int width = rand() % 5 + 3;
        int y = rand() % (LINES - height);
        int x = rand() % (COLS - width);


        Room temp = {x, y, height, width}; // on the stack

        int overlapFlag = 0;
        for (int j = 0; j < created; j++) {
            if (overlap(&temp, rooms[j])) {
                overlapFlag = 1;
                break;
            }
        }

        if (!overlapFlag) {
            rooms[created] = malloc(sizeof(Room));
            *rooms[created] = temp; // copy temp into heap
            created++;
        }
    }

}

Room  * createRoom(int y, int x, int height, int width){
    Room *room = malloc(sizeof(Room));
    room->xpos = x;
    room->ypos = y;
    room->height = height;
    room->width = width;
}


int overlap(Room *room1, Room *room2){
    // check if two rooms overlap
    if(room1->xpos < room2->xpos + room2->width &&
       room1->xpos + room1->width > room2->xpos &&
       room1->ypos < room2->ypos + room2->height &&
       room1->ypos + room1->height > room2->ypos){
        return 1; // overlap
    } else {
        return 0; // no overlap
    }
}

int screenSet(){
    initscr();

    noecho();
    refresh();

    return 0;
}

// Sets up pointer to a pointer?
void *playerSet(Player **player){
    *player = malloc(sizeof(Player));
    (*player)->xpos = 2;
    (*player)->ypos = 3;
    (*player)->hp = 100;

    mvprintw((*player)->ypos, (*player)->xpos, "@");
}

int handleInput(int input, Player *player){
    // move player
    switch(input){

        // Movement buttons
        case 'w':
        case 'W':
            if(checkPosition(player->xpos, player->ypos - 1) == 0){
                playerMove(-1, 0, player);
            } 
            break;
        case 's':
        case 'S':
            if(checkPosition(player->xpos, player->ypos + 1) == 0){
                playerMove(1, 0, player);
            } 
            break;
        case 'a':
        case 'A':
            if(checkPosition(player->xpos - 1, player->ypos) == 0){
                playerMove(0, -1, player);
            } 
            break;
        case 'd':
        case 'D':
            if(checkPosition(player->xpos + 1, player->ypos) == 0){
                playerMove(0, 1, player);
            } 
            break;
        default:
            break;

    }
}

int playerMove(int ypos, int xpos, Player *player){
    mvprintw(player->ypos, player->xpos, ".");  
    player->ypos += ypos;
    player->xpos += xpos;
    mvprintw(player->ypos, player->xpos, "@");  
    move(player->ypos, player->xpos);
}

int checkPosition(int x, int y){
    if (mvinch(y, x) == '-' || mvinch(y, x) == '|') {
        return 1; // collision
    } else {
        return 0; // no collision
    }
}

