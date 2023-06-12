#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <ncurses.h>

#define CLOCK_CHAR '$'

const int charlist[50][5] = {
    // 0
    {1,1,1,1,1},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,1,1,1,1},
    // 1
    {0,1,1,0,0},
    {0,0,1,0,0},
    {0,0,1,0,0},
    {0,0,1,0,0},
    {1,1,1,1,1},
    // 2
    {1,1,1,1,1},
    {0,0,0,0,1},
    {1,1,1,1,1},
    {1,0,0,0,0},
    {1,1,1,1,1},
    // 3
    {1,1,1,1,1},
    {0,0,0,0,1},
    {1,1,1,1,1},
    {0,0,0,0,1},
    {1,1,1,1,1},
    // 4
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,1,1,1,1},
    {0,0,0,0,1},
    {0,0,0,0,1},
    // 5
    {1,1,1,1,1},
    {1,0,0,0,0},
    {1,1,1,1,1},
    {0,0,0,0,1},
    {1,1,1,1,1},
    // 6
    {1,1,1,1,1},
    {1,0,0,0,0},
    {1,1,1,1,1},
    {1,0,0,0,1},
    {1,1,1,1,1},
    // 7
    {1,1,1,1,1},
    {0,0,0,0,1},
    {0,0,0,0,1},
    {0,0,0,0,1},
    {0,0,0,0,1},
    // 8
    {1,1,1,1,1},
    {1,0,0,0,1},
    {1,1,1,1,1},
    {1,0,0,0,1},
    {1,1,1,1,1},
    // 9
    {1,1,1,1,1},
    {1,0,0,0,1},
    {1,1,1,1,1},
    {0,0,0,0,1},
    {0,0,0,0,1},
};

void renderdigit(int digit, int row, int col, int digitnum){
    for(int i = 5 * digit ; i < (5*(digit+1)); i++){
        for(int j = 0; j < 5; j++){
            // FIXME: hacky, make this more readable
            move(row/2 - 4 + (i - digit*5),(digitnum - 1)*(5 + 2) + (col - 29)/2 + j);
            if(charlist[i][j]){
                addch(CLOCK_CHAR);
            }else{
                addch(' ');
            }
        }
    }
}


struct clock{
    struct tm *t;
    struct time{
        int hour;
        int min;
        int sec;
        char* meridiem;
    } time;
    struct date{
        char datestring[80];
    } date;
};

void init(){
    initscr();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED , COLOR_BLACK);
    init_pair(2, COLOR_GREEN , COLOR_BLACK);
    init_pair(3, COLOR_YELLOW , COLOR_BLACK);
    init_pair(4, COLOR_BLUE , COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA , COLOR_BLACK);
    init_pair(6, COLOR_CYAN , COLOR_BLACK);
    init_pair(7, COLOR_WHITE , COLOR_BLACK);
}

void signalhandler(int sig){
}

void update_time(time_t curt){
    time(&curt);
}

int main(){
    struct clock *myclock;
    myclock = (struct clock *)malloc(sizeof(struct clock));

    if ( myclock == NULL ){
        fprintf(stderr, "ERROR: malloc error");
        return -1;
    }


    time_t curt;
    time(&curt);

    myclock->t = localtime(&curt);
    // format: Wed 31 May 2023
    if(strftime(myclock->date.datestring,sizeof(myclock->date.datestring), "%a %e %b %Y", myclock->t) == 0){
        printf("ERROR: strftime returned 0\n");
        return -1;
    };

    char* date = myclock->date.datestring;
    myclock->time.sec = myclock->t->tm_sec;
    myclock->time.min = myclock->t->tm_min;
    myclock->time.hour = myclock->t->tm_hour;

    int row,col;
    bool isrunning = true;
    int ch;
    short color = 1;

    init();
    getmaxyx(stdscr,row,col);

    while (isrunning) {
        attron(COLOR_PAIR(color));
        renderdigit(myclock->time.hour / 10 , row, col, 1);
        renderdigit(myclock->time.hour % 10 , row, col, 2);
        renderdigit(myclock->time.min / 10 , row, col, 3);
        renderdigit(myclock->time.min % 10 , row, col, 4);

        mvprintw(row/2 + 3, (col - strlen(date))/2, "%s", date);
        attroff(COLOR_PAIR(color));

        ch = getch();
        switch(ch) {
            case 'Q':
            case 'q':
                isrunning = false;
                break;
            case '1':
                color = 1;
                break;
            case '2':
                color = 2;
                break;
            case '3':
                color = 3;
                break;
            case '4':
                color = 4;
                break;
            case '5':
                color = 5;
                break;
            case '6':
                color = 6;
                break;
            case '7':
                color = 7;
                break;
            default: 
                break;
        }

        refresh();
    }
    refresh();
    endwin();
    return 0;
}
