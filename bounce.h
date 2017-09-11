#define BLANK ' '
#define DFL_SYMBOL 'o'
#define TOP_ROW 0
#define BOT_ROW 20
#define LEFT_EDGE 0
#define RIGHT_EDGE 81
#define X_INIT 3
#define Y_INIT 5
#define TICKS_PER_SEC 50
#define Y_TIM 8
#define X_TIM 8
#define FLAP "               " 
//#define FLAP_LEN 21
#define FLAP_SPEED 1

struct ppball {
    int x_ttg; // x 轴下次重画还要等待多少个计时器
    int y_ttg; // y 轴下次重画还要等待多少个计时器
    int x_ttm; // x 轴移动需要等待的信号间隔
    int y_ttm; // y 轴移动絮叨等待的信号间隔
    int y_pos;
    int x_pos;
    int y_dir;
    int x_dir;
    int x_moved;
    int y_moved;
    char symbol;
};