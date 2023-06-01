#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <ncurses.h>

int charlist[50][5] = {
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

void renderdigit(int digit, int row, int col){
    for(int i = 5 * digit ; i < (5*(digit+1)); i++){
        for(int j = 0; j < 5; j++){
            move(row/2 - 4 + (i - digit*5),(col - 29)/2 + j);
            if(charlist[i][j]){
                addch('@');
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
    init_pair(1, COLOR_RED, COLOR_BLACK);
}

void signalhandler(int sig){
}

int main(){
    struct clock *myclock;
    myclock = (struct clock *)malloc(sizeof(struct clock));

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

    char* timestring1 = "@@@@@  @@@@@     @@@@@  @@@@@";
    char* timestring2 = "@   @  @   @  0  @   @  @   @";
    char* timestring3 = "@   @  @   @     @   @  @   @";
    char* timestring4 = "@   @  @   @  0  @   @  @   @";
    char* timestring5 = "@@@@@  @@@@@     @@@@@  @@@@@";

    int row,col;
    init();
    getmaxyx(stdscr,row,col);

    int digit = 9;
    renderdigit(digit, row, col);


    /* mvprintw(row/2 - 4,(col - strlen(timestring1))/2,"%s",timestring1); */
    /* mvprintw(row/2 - 3,(col - strlen(timestring2))/2,"%s",timestring2); */
    /* mvprintw(row/2 - 2,(col - strlen(timestring3))/2,"%s",timestring3); */
    /* mvprintw(row/2 - 1,(col - strlen(timestring4))/2,"%s",timestring4); */
    /* mvprintw(row/2,(col - strlen(timestring5))/2,"%s",timestring5); */

    attron(COLOR_PAIR(1));
    mvprintw(row/2 + 3, (col - strlen(date))/2, "%s", date);
    attroff(COLOR_PAIR(1));
    refresh();
    getch();
    endwin();
    return 0;
}
