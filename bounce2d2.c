/*
 * bounce2d 1.0
 * bounce a character (default is 'o') around the screen
 *  defined by some parameters
 * user input: s slow down x component, S: slow y component
 *             f speed up x component, F: speed y component
 *             Q quit
 * blocks on read, but timer tick sends SIGALRM caught by ball_move
 * build: cc bounce2d.c set_ticker.c -lcurses -o bounce2d
    */
#include <curses.h>
#include <string.h>
#include <signal.h>
#include "bounce.h"

struct ppball the_ball;

/** the main loop **/

//int flap_pos = RIGHT_EDGE / 2 - LEFT_EDGE;
int flap_pos =0;
int old_pos = 0;
void set_up();
void wrap_up();
void move_flap();
int bounce_or_lose(struct ppball *);

int main()
{
//    printf("LEFT_EDGE: %d, RIGHT_EDGE: %d\n",LEFT_EDGE, RIGHT_EDGE);
//    printf("TOP_EDGE: %d, BOT_EDGE: %d\n",TOP_ROW, BOT_ROW);
    int c;
    set_up();
    while (((c = getchar())) != 'Q')
    {
        if (c == 'f') the_ball.x_ttm--;
        else if (c == 's') the_ball.x_ttm++;
        else if (c == 'F') the_ball.y_ttm--;
        else if (c == 'S') the_ball.y_ttm++;
        else if (c == 'a'){
            if (flap_pos > LEFT_EDGE){
                old_pos = flap_pos;
                flap_pos -= FLAP_SPEED;
                move_flap();
            }
        }else if (c == 'd'){
            if (flap_pos + (int)strlen(FLAP) < RIGHT_EDGE){
                old_pos = flap_pos;
                flap_pos += FLAP_SPEED;
                move_flap();
            }
        }
    }
    wrap_up();
//    printf("LINES: %d, COLS: %d\n",LINES, COLS);
    return 0;
}

void set_up()
    /*
     * init structure and other stuff
     */
{
    void ball_move(int);
    the_ball.y_pos = Y_INIT;
    the_ball.x_pos = X_INIT;
    the_ball.y_ttg = the_ball.y_ttm = Y_TIM;
    the_ball.x_ttg = the_ball.x_ttm = X_TIM;
    the_ball.y_dir = 1;
    the_ball.x_dir = 1;
    the_ball.symbol = DFL_SYMBOL;
    the_ball.x_moved = the_ball.y_moved = false;

    initscr();
    noecho();
    crmode();

    signal(SIGINT, SIG_IGN);
    mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
    move_flap();
    signal(SIGALRM, ball_move);
    set_ticker(1000 / TICKS_PER_SEC);
}

void wrap_up()
{
    set_ticker(0);
    endwin();
}

void move_flap()
{
    move(BOT_ROW + 1, old_pos);
    addstr(FLAP); //FLAP is blank. Here it is used to clear its old existence.

    move(BOT_ROW + 1, flap_pos);
    standout();
    addstr(FLAP);
    standend();
    refresh();
}

void ball_move(int signum)
{
    int y_cur, x_cur, moved;

    signal(SIGALRM, SIG_IGN);
    x_cur = the_ball.x_pos;
    y_cur = the_ball.y_pos;
    moved = 0;

    if (the_ball.y_ttm > 0 &&  the_ball.y_ttg-- == 1){
        the_ball.y_pos += the_ball.y_dir;  /* move */
        the_ball.y_ttg = the_ball.y_ttm;   /* reset */
        the_ball.y_moved = 1;
        moved = 1;
    }

    if (the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1){
        the_ball.x_pos += the_ball.x_dir;  /* move */
        the_ball.x_ttg = the_ball.x_ttm;   /* reset */
        the_ball.x_moved = 1;
        moved = 1;
    }

    if (moved){
        mvaddch(y_cur, x_cur, BLANK);
        mvaddch(y_cur, x_cur, BLANK);
        mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
        if(bounce_or_lose(&the_ball)){
//            signal(SIGALRM, SIG_IGN);
            move(LINES / 2, COLS / 2);
            addstr("GAME OVER");
            refresh();
            return;
        }
        move(LINES-1, COLS-1);
        if (the_ball.x_moved && the_ball.y_moved){
            refresh();
            the_ball.x_moved = the_ball.y_moved = false; /* reset */
        }
    }
    signal(SIGALRM, ball_move);
}

int bounce_or_lose(struct ppball *bp)
    /*
     * 1 lose
     * 0 not lose
     */
{
    int return_val = 0;

    if (bp->y_pos == TOP_ROW){
        bp->y_dir = 1;
    }else if (bp->y_pos == BOT_ROW){
        bp->y_dir = -1;
        if (!(bp->x_pos >= flap_pos && bp->x_pos <= (flap_pos + (int)strlen(FLAP)))){
            return_val = 1;
        }
    }

    if (bp->x_pos == LEFT_EDGE){
        bp->x_dir = 1;
    }else if (bp->x_pos == RIGHT_EDGE){
        bp->x_dir = -1;
    }
    return return_val;
}