#include <ncurses.h>
#include <iostream>
#include <ctime>
#include <time.h>
#include <locale.h>
#include <list>
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

    queue<pair<int,int>> getBody(){
        return body;
    }

    int getLength(){
        return length;
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
    bool move_PlusBody(){
        int popy = body.front().first;
        int popx = body.front().second;

        length++; // 바디값이 늘어난 것을 명시, item 클래스에 바디 길이를 체크할 때 활용하기 때문에.

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

    bool move_MinusBody(){
        int popy = body.front().first;
        int popx = body.front().second;

        body.pop();
        body.pop();
        length--; // 바디값이 늘어난 것을 명시, item 클래스에 바디 길이를 체크할 때 활용하기 때문에.

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
        if(map[heady][headx]==1){
            return false;
        }

        return true;
    }
};

// value 5
class Item{
    int grow_x, grow_y, poison_x, poison_y;
    int cnt;
    int bodyLength;
    int body[][2];

public:
    Item(){
        grow_x = 0; grow_y = 0;
        poison_x = 20; poison_y = 20;
    }
    
    // snake의 바디값 설정
    void setBody(queue<pair<int, int>> snake, int length){
        bodyLength = length;
        for (int i=0; i< bodyLength; i++){
            pair<int, int> top = snake.front();
            body[i][0] = top.first;
            body[i][1] = top.second;
            snake.pop();
        }
    }
    
    // 바디값이 아닌 위치를 범위 내에서 랜덤 설정(1~19)
    // posisonItem이랑 위치 겹치면 안되기 때문에 main에서 비교 후 다시 설정해주는 작업해야됨
    pair<int,int> getGrowItemPosition(){
        srand(time(NULL));
        while (true){
            int y = rand() % 20;
            int x = rand() % 20;
            if (positionIsSuited(y,x) && (y != poison_y && x != poison_x)){
                grow_y = y;
                grow_x = x;
                return make_pair(y,x);
            }
        }
    }

    pair<int,int> getPoisonItemPosition(){
        srand(time(NULL));
        while (true){
            int y = rand() % 20;
            int x = rand() % 20;
            if (positionIsSuited(y,x) && (y != grow_y && x != grow_x)){
                poison_y = y;
                poison_x = x;
                return make_pair(y,x);
            }
        }
    }

    bool positionIsSuited(int y, int x){
        if (y !=0 && x !=0){
            for (int i=0; i< bodyLength; i++){
                if (y == body[i][0] || x == body[i][1]){
                    return false;
                }
            }
        }
        return true;
    }
};

// value 6

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
    //초기맵 설정
    Item item = Item();
    item.setBody(sk.getBody(), sk.getLength());
    
    pair<int, int> growitem = item.getGrowItemPosition();
    map[growitem.first][growitem.second] = 5;

    pair<int, int> poisonitem = item.getPoisonItemPosition();
    map[poisonitem.first][poisonitem.second] = 6;

    while (1) {
        bool flag = false;
            // 지렁이의 헤드가 포지션을 지나면 새로운 값 생성 후 지렁이 사이즈 하나 늘림
            // 개선점 : 헤드가 먹이를 먹자마자 새로운 grow item이 생겨야 하고 바로 body 사이즈가 늘어나야함.
            if (growitem.first == sk.getBody().front().first && growitem.second == sk.getBody().front().second){
                map[growitem.first][growitem.second] = 0;
                item.setBody(sk.getBody(), sk.getLength());
                growitem = item.getGrowItemPosition();
                map[growitem.first][growitem.second] = 5;
                if (!sk.move_PlusBody()){
                    break;
                }
                flag = true;
            }
            // 포인즌 아이템이 안뜸 초기에 뜨다가 말음. 
            if (poisonitem.first == sk.getBody().front().first && poisonitem.second == sk.getBody().front().second){
                map[poisonitem.first][poisonitem.second] = 0;
                item.setBody(sk.getBody(), sk.getLength());
                poisonitem = item.getPoisonItemPosition();
                map[poisonitem.first][poisonitem.second] = 6;
                if (!sk.move_MinusBody()){
                    break;
                }
                flag = true;
            }

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
        
        // 5초가 지나면 아이템 재설정하는 건 민재형이랑 코드리뷰하고 설정
        if((float)(endClock - startClock)/CLOCKS_PER_SEC >= 0.4){
            if ((!flag) && !sk.move()) {
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
