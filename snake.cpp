#include <ncurses.h>
#include <iostream>
#include <ctime>
#include <locale.h>
#include <queue>
#define MAP_SIZE 21

// 해야될 것
// 다양한 맵 만들기
// item 먹는 것
// 차원의 문(?) -> 이동까지 구현필요
// 게임 종료시
// 포인트와 미션 구현

using namespace std;
enum direction {WEST, NORTH, EAST, SOUTH}; //서: 0 북:1 동:2 남:3

int map[MAP_SIZE][MAP_SIZE];
class Point{
    int currentLength = 3;
    int maxLength = 3;
    int B = currentLength / maxLength;
    int growItem = 0;
    int poisonItem = 0;
    int useGate = 0;

    int mission_B;
    int mission_growItem;
    int mission_poisonItem;
    int mission_useGate;
};

class Snake{
    int curDirection = WEST;
    int headx, heady;
    int length = 3;
    queue<pair<int,int> > body; // (y,x)

public:
    Snake(int map_size){
        heady = map_size/2;
        headx = map_size/2-1;
        body.push({map_size/2, map_size/2+1});
        body.push({map_size/2, map_size/2});
        body.push({map_size/2, map_size/2-1});
    }

    //입력된 키를 기준으로 방향전환
    //방향전환시 죽는 경우 있음
    bool turnDirection(int key){
        if (abs(key - curDirection) == 2) return false;
        // else if(curDirection==WEST){
            

        // }else if(curDirection==NORTH){

        // }else if(curDirection==EAST){

        // }else if(curDirection==SOUTH){

        // }
        curDirection = key;
        return true;
    }

    //현재 방향을 기준으로 움직임
    bool move(){
        int popy = body.front().first;
        int popx = body.front().second;
        body.pop();
        map[popy][popx] = 0;
        map[heady][headx] = 4;

        if(curDirection==0){
            headx-=1;
        }
        else if(curDirection == 1){
            heady-=1;
        }
        else if(curDirection == 2){
            headx +=1;
        }
        else{
            heady+=1;
        }

        //벽이나 어떤 이벤트 생기나 확인
        if(!check()){
            return false;
        }

        map[heady][headx] = 3;
        body.push({heady,headx});
        return true;
    }

    bool check(){
        if(map[heady][headx]==1){ // 벽
            return false;
        }

        return true;
    }
};


class growItem{
    int x, y;
    static int cnt;
    int time = 0;

    // void timeout(){
     
    // }
};

class poisonItem{
    int x, y;
    static int cnt;
    int time = 0;

    // void timeout(){
    //     if(time>=5){

    //     }
    // }
};

void map_init();

int kbhit(void){
    int ch = getch();
    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

void map_random_color(){
    srand(time(NULL));

    int x = rand();
    int y = rand();
    x %= MAP_SIZE;
    y %= MAP_SIZE;
    while(map[x][y]!=0){
        x = rand();
        y = rand();
        x %= MAP_SIZE;
        y %= MAP_SIZE;
    }
    // cout << x << ' ' << y <<'\n';
    map[x][y] = 1;
}

//0.5초 sleep이 아닌 다른 구현 필요
void sleep(float seconds){
    clock_t startClock = clock();
    float secondsAhead = seconds * CLOCKS_PER_SEC;
    // do nothing until the elapsed time has passed.
    while(clock() < startClock+secondsAhead);
    return;
}

void draw_snakewindow(WINDOW *snake_win);

int main(){
    WINDOW *snake_win;
    WINDOW *point_win;

    Snake sk = Snake(MAP_SIZE);

    setlocale(LC_ALL, "");
    map_init();

    initscr();
    resize_term(60,100);

    start_color();
    init_pair(1, COLOR_RED, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_BLUE, COLOR_CYAN);
    init_pair(4,COLOR_GREEN, COLOR_MAGENTA);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);

    border('*','*','*','*','*','*','*','*');
    mvprintw(1,1, "SNAKE GAME");
    refresh();


    //디스플레이 하드코딩 된 부분 수정 필요
    snake_win = newwin(30,60,3,3);
    wbkgd(snake_win,COLOR_PAIR(5));
    wattron(snake_win, COLOR_PAIR(5));
    mvwprintw(snake_win, 1,1, "Snake_game window");
    wborder(snake_win, '|','|','-','-','+','+','+','+');
    wrefresh(snake_win);

    point_win = newwin(21,21,3,65);
    wbkgd(point_win,COLOR_PAIR(5));
    wattron(point_win, COLOR_PAIR(5));
    mvwprintw(point_win, 1,1, "Score_board");
    wborder(point_win, '|','|','-','-','+','+','+','+');
    wrefresh(point_win);



    int key;
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    //0.5초마다 map 반영해서 그리기
    //여기가 좀 복잡한 문제
    //쓰레드를 따로 둬서 0.5초 동안 입력을 받을 수 있게끔 정밀한 구현이 필요함
    //getch()를 받기 까지 blocking되는 이슈
    //select? 
    //키입력 -> 방향전환 -> 움직임


    clock_t startClock;
    clock_t endClock;

    startClock = clock();
    while (1) {
        if (kbhit()) {
            key = getch();
            if(key==259){
                // printw("up");
                sk.turnDirection(NORTH);
            }
            else if(key==260){
                // printw("left");
                sk.turnDirection(WEST);
            }
            else if(key==258){             
                // printw("down");
                sk.turnDirection(SOUTH);
            }
            else if(key == 261){
                // printw("right");
                sk.turnDirection(EAST);
            }
            refresh();
        } 
        endClock = clock();
        
        if((float)(endClock - startClock)/CLOCKS_PER_SEC >= 0.4){
            if (!sk.move()) {
                break;
            };
            startClock = endClock;
            draw_snakewindow(snake_win);
            wrefresh(snake_win);
        }
    }

    // wborder(snake_win, '|','|','-','-','+','+','+','+');
    getch();
    delwin(snake_win);
    endwin();
    return 0;
}

void map_init(){
    for(int i=0; i<MAP_SIZE; i++){
        map[i][MAP_SIZE-1] = 1;
        map[i][0] = 1;
        map[MAP_SIZE-1][i] = 1;
        map[0][i] = 1;
    }
    map[0][0] = 2;
    map[0][MAP_SIZE-1] = 2;
    map[MAP_SIZE-1][MAP_SIZE-1] = 2;
    map[MAP_SIZE-1][0] = 2;

    map[MAP_SIZE/2][MAP_SIZE/2] = 3;
    map[MAP_SIZE/2][MAP_SIZE/2+1] = 3;
    map[MAP_SIZE/2][MAP_SIZE/2-1] = 4;
}

void draw_snakewindow(WINDOW *snake_win) {
    for (int i = 3; i < MAP_SIZE + 3; i++) {
        int j = 3;
        for (; j < MAP_SIZE + 3; j++) {
            if (map[i - 3][j - 3] == 0) {
                mvwprintw(snake_win, i, j * 2, "\u2B1C");
            }
            else if (map[i - 3][j - 3] == 1) {
                mvwprintw(snake_win, i, j * 2, "\u2B1B");
            }
            else if (map[i - 3][j - 3] == 2) {
                mvwprintw(snake_win, i, j * 2, "\u2B1B");
            }
            else if (map[i - 3][j - 3] == 3) {
                mvwprintw(snake_win, i, j * 2, "\u2B1B");
            }
            else if (map[i - 3][j - 3] == 4) {
                mvwprintw(snake_win, i, j * 2, "\u2B1B");
            }
            else if (map[i - 3][j - 3] == 5) {
                mvwprintw(snake_win, i, j * 2, "\u2B1B");
            }
            else if (map[i - 3][j - 3] == 5) {
                mvwprintw(snake_win, i, j * 2, "\u2B1B");
            }
            else if (map[i - 3][j - 3] == 5) {
                mvwprintw(snake_win, i, j * 2, "\u2B1B");
            }
        }
        mvwprintw(snake_win, i, j * 2, "\n");
    }
}
