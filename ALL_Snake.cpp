#include <ncurses.h>
#include <iostream>
#include <ctime>
#include <time.h>
#include <locale.h>
#include <list>
#include <deque>
#define MAP_SIZE 21

// 해야될 것
// 다양한 맵 만들기
// item 먹는 것
// 차원의 문(?) -> 이동까지 구현필요
// 게임 종료시
// 포인트와 미션 구현

using namespace std;
enum direction { WEST, NORTH, EAST, SOUTH }; //서: 0 북:1 동:2 남:3

int map[MAP_SIZE][MAP_SIZE];
int walls[5][2] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };

class Point {
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

class Snake {
public:
    int curDirection = WEST;
    int headx, heady;
    int length = 3;
    deque<pair<int, int> > body; // (y,x)

public:
    Snake(int map_size) {
        heady = map_size / 2;
        headx = map_size / 2 - 1;
        body.push_back({ map_size / 2, map_size / 2 + 1 });
        body.push_back({ map_size / 2, map_size / 2 });
        body.push_back({ map_size / 2, map_size / 2 - 1 });
    }

    int gate[2][2] = { { 0, 0 }, { 0, 0 } }; // [ [y1, x1], [y2, x2] ]
    // 게이트 랜덤생성을 위한 카운트
    // int waitGate = rand() % 10 + 10;
    int waitGate = 1;

    void setPosition(int& x, int& y)
    {
        int isSide = (rand() % 2);
        int xORy = (rand() % 2);
        int zeroOr20 = (rand() % 2);
        if (isSide)
        {
            // cout << "inside " << endl;
            if (xORy)
            {
                if (zeroOr20)
                {
                    x = 20;
                    y = rand() % 19 + 1;
                }
                else
                {
                    x = 0;
                    y = rand() % 19 + 1;
                }
            }
            else
            {
                if (zeroOr20)
                {
                    x = rand() % 19 + 1;
                    y = 20;
                }
                else
                {
                    x = rand() % 19 + 1;
                    y = 0;
                }
            }
        }
        else
        {
            // cout << "outSide " << endl;
            int choose = (rand() % 5);
            y = walls[choose][0];
            x = walls[choose][1];
        }
    }

    void makeGate()
    {
        int x1, y1, x2, y2;
        x1 = y1 = x2 = y2 = 0;
        while (x1 == x2 and y1 == y2)
        {
            setPosition(x1, y1);
            setPosition(x2, y2);
        };
        gate[0][0] = y1;
        gate[0][1] = x1;
        gate[1][0] = y2;
        gate[1][1] = x2;
        // cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
    }



    //item 관련 

    deque<pair<int, int>> getBody() {
        return body;
    }

    int getLength() {
        return length;
    }
    //입력된 키를 기준으로 방향전환
    //방향전환시 죽는 경우 있음
    bool turnDirection(int key) {
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
    bool move() {
        int pop_fronty = body.front().first;
        int pop_frontx = body.front().second;
        
        map[pop_fronty][pop_frontx] = 0;
        map[heady][headx] = 4;

        if (curDirection == 0) {
            headx -= 1;
        }
        else if (curDirection == 1) {
            heady -= 1;
        }
        else if (curDirection == 2) {
            headx += 1;
        }
        else {
            heady += 1;
        }
        
        // item 만났을 때 바디값 조절
        if(map[heady][headx] == 5){
            map[pop_fronty][pop_frontx] = 4;
            length++;// 바디값이 늘어난 것을 명시, item 클래스에 바디 길이를 체크할 때 활용하기 때문에.
        }
        else if (map[heady][headx] == 6){
            body.pop_front();
            int body_lasty = body.front().first;
            int body_lastx = body.front().second;
            body.pop_front();
            map[body_lasty][body_lastx] = 0;
            length--;
        }
        else{
            body.pop_front();
        }
        
        //map, wall
        if (map[heady][headx] == 1)
        { // 벽
            return false;
        }

        // count wait gate
        if (waitGate == 0)
        {
            makeGate();
        }
        waitGate--;
        if (map[heady][headx] == 7)
        {
            headx = (headx == gate[0][1]) ? gate[1][1] : gate[0][1];
            heady = (heady == gate[0][0]) ? gate[1][0] : gate[0][0];

            // 게이트가 벽에 있을 때 방향 조절
            if (headx == 0)
                curDirection = EAST;
            if (headx == 20)
                curDirection = WEST;
            if (heady == 0)
                curDirection = SOUTH;
            if (heady == 20)
                curDirection = NORTH;

            // 게이트가 맵 가운데에 있을 때
            // 원래 방향이 동쪽일 때
            if (curDirection == EAST && map[heady][headx + 1] == 1)
            {
                if (map[heady + 1][headx] == 1)
                {
                    if (map[heady - 1][headx] == 1)
                    {
                        if (map[heady][headx - 1] == 1)
                        {
                            // cout << "SETTING DIRECTION ERROR" << endl;
                        }
                        else
                        {
                            curDirection = WEST;
                        }
                    }
                    else
                    {
                        curDirection = NORTH;
                    }
                }
                else
                {
                    curDirection = SOUTH;
                }
            }
            // 원래 방향이 서쪽일 때
            else if (curDirection == WEST && map[heady][headx - 1] == 1)
            {
                if (map[heady - 1][headx] == 1)
                {
                    if (map[heady + 1][headx] == 1)
                    {
                        if (map[heady][headx + 1] == 1)
                        {
                            // cout << "SETTING DIRECTION ERROR" << endl;
                        }
                        else
                        {
                            curDirection = EAST;
                        }
                    }
                    else
                    {
                        curDirection = SOUTH;
                    }
                }
                else
                {
                    curDirection = NORTH;
                }
            }
            // 원래 방향이 남쪽일 때
            else if (curDirection == SOUTH && map[heady + 1][headx] == 1)
            {
                if (map[heady][headx - 1] == 1)
                {
                    if (map[heady][headx + 1] == 1)
                    {
                        if (map[heady - 1][headx] == 1)
                        {
                            // cout << "SETTING DIRECTION ERROR" << endl;
                        }
                        else
                        {
                            curDirection = NORTH;
                        }
                    }
                    else
                    {
                        curDirection = EAST;
                    }
                }
                else
                {
                    curDirection = WEST;
                }
            }
            // 원래 방향이 북쪽일 때
            else if (curDirection == NORTH && map[heady - 1][headx] == 1)
            {
                if (map[heady][headx + 1] == 1)
                {
                    if (map[heady][headx - 1] == 1)
                    {
                        if (map[heady + 1][headx] == 1)
                        {
                            // cout << "SETTING DIRECTION ERROR" << endl;
                        }
                        else
                        {
                            curDirection = SOUTH;
                        }
                    }
                    else
                    {
                        curDirection = WEST;
                    }
                }
                else
                {
                    curDirection = EAST;
                }
            }
        }
        
        
        //벽이나 어떤 이벤트 생기나 확인
        if (!check()) {
            return false;
        }

        map[heady][headx] = 3;
        body.push_back({ heady,headx });
        return true;
    }

    bool check() {
        if (map[heady][headx] == 1) {
            return false;
        }

        return true;
    }
};

// value 5
class Item {
    int grow_x, grow_y, poison_x, poison_y;
    int cnt;
    int bodyLength;
    int body[][2];

public:
    Item() {
        grow_x = 0; grow_y = 0;
        poison_x = 20; poison_y = 20;
    }

    // snake의 바디값 설정
    void setBody(deque<pair<int, int>> snake, int length) {
        bodyLength = length;
        for (int i = 0; i < bodyLength; i++) {
            pair<int, int> top = snake.front();
            body[i][0] = top.first;
            body[i][1] = top.second;
            // snake.pop_front();
        }
    }

    // 바디값이 아닌 위치를 범위 내에서 랜덤 설정(1~19)
    // posisonItem이랑 위치 겹치면 안되기 때문에 main에서 비교 후 다시 설정해주는 작업해야됨
    pair<int, int> getGrowItemPosition() {
        srand(time(NULL));
        while (true) {
            int y = rand() % 20;
            int x = rand() % 20;
            if (positionIsSuited(y, x) && (y != poison_y && x != poison_x)) {
                grow_y = y;
                grow_x = x;
                return make_pair(y, x);
            }
        }
    }

    pair<int, int> getPoisonItemPosition() {
        srand(time(NULL));
        while (true) {
            int y = rand() % 20;
            int x = rand() % 20;
            if (positionIsSuited(y, x) && (y != grow_y && x != grow_x)) {
                poison_y = y;
                poison_x = x;
                return make_pair(y, x);
            }
        }
    }

    bool positionIsSuited(int y, int x) {
        if (y != 0 && x != 0) {
            for (int i = 0; i < bodyLength; i++) {
                if (y == body[i][0] || x == body[i][1]) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

// value 6

void map_init();

int kbhit(void) {
    int ch = getch();
    if (ch != ERR) {
        ungetch(ch);
        return 1;
    }
    else {
        return 0;
    }
}

void map_random_color() {
    srand(time(NULL));

    int x = rand();
    int y = rand();
    x %= MAP_SIZE;
    y %= MAP_SIZE;
    while (map[x][y] != 0) {
        x = rand();
        y = rand();
        x %= MAP_SIZE;
        y %= MAP_SIZE;
    }
    // cout << x << ' ' << y <<'\n';
    map[x][y] = 1;
}

//0.5초 sleep이 아닌 다른 구현 필요
void sleep(float seconds) {
    clock_t startClock = clock();
    float secondsAhead = seconds * CLOCKS_PER_SEC;
    // do nothing until the elapsed time has passed.
    while (clock() < startClock + secondsAhead);
    return;
}

const char* returnScore(int score){
    string tmp = to_string(score);
    char const *result = tmp.c_str();
    return result;
}

void draw_snakewindow(WINDOW* snake_win);

int main() {
    srand(time(NULL));

    WINDOW* snake_win;
    WINDOW* point_win;
    WINDOW* mission_win;

    int map1[5][2] = { { 2, 2 }, { 2, 3 }, { 2, 4 }, { 2, 5 }, { 2, 6 } };
    int map2[5][2] = { { 3, 2 }, { 3, 3 }, { 3, 4 }, { 3, 5 }, { 3, 6 } };
    int map3[5][2] = { { 5, 2 }, { 5, 3 }, { 5, 4 }, { 5, 5 }, { 5, 6 } };
    int map4[5][2] = { { 5, 2 }, { 5, 3 }, { 5, 4 }, { 5, 5 }, { 6, 6 } };
    int map5[5][2] = { { 6, 2 }, { 6, 3 }, { 6, 4 }, { 6, 5 }, { 6, 6 } };

    int chooseMap = (rand() % 5);
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 2; j++)
            switch (chooseMap)
            {
            case 0:
                walls[i][j] = map1[i][j];
                break;
            case 1:
                walls[i][j] = map2[i][j];
                break;
            case 2:
                walls[i][j] = map3[i][j];
                break;
            case 3:
                walls[i][j] = map4[i][j];
                break;
            case 4:
                walls[i][j] = map5[i][j];
                break;
            }
    }

    Snake sk = Snake(MAP_SIZE);

    setlocale(LC_ALL, "");
    map_init();

    initscr();
    resize_term(60, 100);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(4, COLOR_CYAN, COLOR_CYAN);
    init_pair(5, COLOR_GREEN, COLOR_GREEN);
    init_pair(6, COLOR_RED, COLOR_RED);
    init_pair(7, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(8, COLOR_BLACK, COLOR_WHITE);

    border('*', '*', '*', '*', '*', '*', '*', '*');
    mvprintw(1, 1, "SNAKE GAME");
    refresh();

    //디스플레이 하드코딩 된 부분 수정 필요
    snake_win = newwin(30, 60, 3, 3);
    wbkgd(snake_win, COLOR_PAIR(1));
    wattron(snake_win, COLOR_PAIR(8));
    mvwprintw(snake_win, 1, 1, "Snake_game window");
    wborder(snake_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(snake_win);

    
    int growthInteger = 0;
    string tmp_grow = to_string(growthInteger);
    char const *growthChar = tmp_grow.c_str();

    int poisonInteger = 0;
    string tmp_poison = to_string(poisonInteger);
    char const *poisonChar = tmp_poison.c_str();

    point_win = newwin(15, 29, 3, 64);
    wbkgd(point_win, COLOR_PAIR(1));
    wattron(point_win, COLOR_PAIR(8));
    
    mvwprintw(point_win, 1, 1, "Score_board");
    mvwprintw(point_win, 2, 1, "B: 3 / 3");
    mvwprintw(point_win, 3, 1, "+: "); mvwprintw(point_win, 3, 5, growthChar);
    mvwprintw(point_win, 4, 1, "-: "); mvwprintw(point_win, 4, 5, poisonChar);
    mvwprintw(point_win, 5, 1, "G: 사용 횟수");

    wborder(point_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(point_win);


    mission_win = newwin(15, 29, 18, 64);
    wbkgd(mission_win, COLOR_PAIR(1));
    wattron(mission_win, COLOR_PAIR(8));
    mvwprintw(mission_win, 1, 1, "Mission");
    
    mvwprintw(mission_win, 2, 1, "B: 10 ("); mvwprintw(mission_win, 2, 9, ")");
    mvwprintw(mission_win, 3, 1, "+: 5  ("); mvwprintw(mission_win, 3, 9, ")");
    mvwprintw(mission_win, 4, 1, "-: 2  ("); mvwprintw(mission_win, 4, 9, ")");
    mvwprintw(mission_win, 5, 1, "G: 1  ("); mvwprintw(mission_win, 5, 9, ")");

    
    wborder(mission_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(mission_win);


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
    
    int countSecond = 0;
    int snakeBodyMaxLength = 3;

    while (1) {
        for (int i = 0; i < 5; i++)
        {
            map[walls[i][0]][walls[i][1]] = 1;
        }
        // view gate
        if ((sk.gate[0][0] + sk.gate[0][1] + sk.gate[1][0] + sk.gate[0][1]) > 0)
        {
            map[sk.gate[0][0]][sk.gate[0][1]] = 7;
            map[sk.gate[1][0]][sk.gate[1][1]] = 7;
        }




        if (sk.length < 3){
            break;
        }
        if (kbhit()) {
            key = getch();
            if (key == 259) {
                // printw("up");
                sk.turnDirection(NORTH);
            }
            else if (key == 260) {
                // printw("left");
                sk.turnDirection(WEST);
            }
            else if (key == 258) {
                // printw("down");
                sk.turnDirection(SOUTH);
            }
            else if (key == 261) {
                // printw("right");
                sk.turnDirection(EAST);
            }
            refresh();
        }
        endClock = clock();

        // 지렁이의 헤드가 포지션을 지나면 새로운 값 생성 후 지렁이 사이즈 하나 늘림
        // 개선점 : 헤드가 먹이를 먹자마자 새로운 grow item이 생겨야 하고 바로 body 사이즈가 늘어나야함.
        if (growitem.first == sk.heady && growitem.second == sk.headx) {
            map[growitem.first][growitem.second] = 0;
            item.setBody(sk.getBody(), sk.getLength());
            growitem = item.getGrowItemPosition();
            map[growitem.first][growitem.second] = 5;

            
            growthChar = returnScore(++growthInteger);
            mvwprintw(point_win, 3, 5, growthChar);
            mvwprintw(point_win, 2, 4, returnScore(sk.getLength()));

            if (sk.getLength() > snakeBodyMaxLength){
                snakeBodyMaxLength = sk.getLength();
                mvwprintw(point_win, 2, 8, returnScore(snakeBodyMaxLength));
            }
            wrefresh(point_win);
        }

        // 포인즌 아이템이 안뜸 초기에 뜨다가 말음. 
        if (poisonitem.first == sk.heady && poisonitem.second == sk.headx) {
            map[poisonitem.first][poisonitem.second] = 0;
            item.setBody(sk.getBody(), sk.getLength());
            poisonitem = item.getPoisonItemPosition();
            map[poisonitem.first][poisonitem.second] = 6;

            poisonChar = returnScore(++poisonInteger);
            mvwprintw(point_win, 4, 5, poisonChar);
            mvwprintw(point_win, 2, 4, returnScore(sk.getLength()));
            wrefresh(point_win);
        }

        if (snakeBodyMaxLength == 10){
            mvwprintw(mission_win, 2, 8, "V");mvwprintw(mission_win, 2, 9, ")");
        }

        if (growthInteger == 5){
            mvwprintw(mission_win, 3, 8, "V");mvwprintw(mission_win,3, 9, ")");
            wrefresh(mission_win);
        }

        if (poisonInteger == 2){
            mvwprintw(mission_win, 4, 8, "V");mvwprintw(mission_win, 4, 9, ")");
            wrefresh(mission_win);
        }

        if (countSecond == 30){
            map[growitem.first][growitem.second] = 0;
            item.setBody(sk.getBody(), sk.getLength());
            growitem = item.getGrowItemPosition();
            map[growitem.first][growitem.second] = 5;

            map[poisonitem.first][poisonitem.second] = 0;
            item.setBody(sk.getBody(), sk.getLength());
            poisonitem = item.getPoisonItemPosition();
            map[poisonitem.first][poisonitem.second] = 6;

            countSecond = 0;
        }

        // 5초가 지나면 아이템 재설정하는 건 민재형이랑 코드리뷰하고 설정
        if ((float)(endClock - startClock) / CLOCKS_PER_SEC >= 0.5) {
            countSecond++;
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

void map_init() {
    for (int i = 0; i < MAP_SIZE; i++) {
        map[i][MAP_SIZE - 1] = 1;
        map[i][0] = 1;
        map[MAP_SIZE - 1][i] = 1;
        map[0][i] = 1;
    }
    map[0][0] = 2;
    map[0][MAP_SIZE - 1] = 2;
    map[MAP_SIZE - 1][MAP_SIZE - 1] = 2;
    map[MAP_SIZE - 1][0] = 2;

    map[MAP_SIZE / 2][MAP_SIZE / 2] = 3;
    map[MAP_SIZE / 2][MAP_SIZE / 2 + 1] = 3;
    map[MAP_SIZE / 2][MAP_SIZE / 2 - 1] = 4;
}
// 10
// 90
// "9 "
void draw_snakewindow(WINDOW* snake_win) {

    for (int i = 4; i < MAP_SIZE + 4; i++) {
        int j = 4;
        for (; j < MAP_SIZE + 4; j++) {
            if (map[i - 4][j - 4] == 0) { //white(빈칸)
                wattron(snake_win, COLOR_PAIR(1));
                mvwprintw(snake_win, i, j * 2, "  ");
                // attroff(COLOR_PAIR(1));
            }
            else if (map[i - 4][j - 4] == 1 || map[i - 4][j - 4] == 2) { //wall
                wattron(snake_win, COLOR_PAIR(2));
                mvwprintw(snake_win, i, j * 2, "  ");
                // attroff(COLOR_PAIR(2));
            }
            else if (map[i - 4][j - 4] == 3) {  //head of snake
                wattron(snake_win, COLOR_PAIR(3));
                mvwprintw(snake_win, i, j * 2, "  ");
                // attroff(COLOR_PAIR(3));
            }
            else if (map[i - 4][j - 4] == 4) {  //head of snake
                wattron(snake_win, COLOR_PAIR(4));
                mvwprintw(snake_win, i, j * 2, "  ");
            }
            else if (map[i - 4][j - 4] == 5) {  // growth item
                wattron(snake_win, COLOR_PAIR(5));
                mvwprintw(snake_win, i, j * 2, "  ");
            }
            else if (map[i - 4][j - 4] == 6) {  //posion item
                wattron(snake_win, COLOR_PAIR(6));
                mvwprintw(snake_win, i, j * 2, "  ");
            }
            else if (map[i - 4][j - 4] == 7) {  //Gate
                wattron(snake_win, COLOR_PAIR(7));
                mvwprintw(snake_win, i, j * 2, "  ");
            }
            else if (map[i - 4][j - 4] == 8) {  //Gate
                wattron(snake_win, COLOR_PAIR(2));
                mvwprintw(snake_win, i, j * 2, "  ");
            }
        }
        mvwprintw(snake_win, i, j * 2, "\n");
    }
}