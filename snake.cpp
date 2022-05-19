#include <cstdlib>
#include <ctime>
#include <iostream>
#include <locale.h>
#include <ncurses.h>
#include <queue>
#define MAP_SIZE 21

// 해야될 것
// 다양한 맵 만들기
// item 먹는 것
// 차원의 문(?) -> 이동까지 구현필요
// 게임 종료시
// 포인트와 미션 구현

using namespace std;
enum direction
{
    WEST,
    NORTH,
    EAST,
    SOUTH
}; //서: 0 북:1 동:2 남:3

int map[MAP_SIZE][MAP_SIZE];
class Point
{
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

class Snake
{
    int curDirection = WEST;
    int headx, heady;
    int length = 3;
    queue<pair<int, int>> body; // (y,x)

public:
    // 게이트는 하나만 존재하면 된다.  snake 객체에 정의
    int gate[2][2]; // [ [y1, x1], [y2, x2] ]
    Snake(int map_size)
    {
        heady = map_size / 2;
        headx = map_size / 2 - 1;
        body.push({ map_size / 2, map_size / 2 + 1 });
        body.push({ map_size / 2, map_size / 2 });
        body.push({ map_size / 2, map_size / 2 - 1 });
    }

    //입력된 키를 기준으로 방향전환
    //방향전환시 죽는 경우 있음
    bool turnDirection(int key)
    {
        if (abs(key - curDirection) == 2)
            return false;
        // else if(curDirection==WEST){

        // }else if(curDirection==NORTH){

        // }else if(curDirection==EAST){

        // }else if(curDirection==SOUTH){

        // }
        curDirection = key;
        return true;
    }

    //현재 방향을 기준으로 움직임
    bool move()
    {
        int popy = body.front().first;
        int popx = body.front().second;
        body.pop();
        map[popy][popx] = 0;
        map[heady][headx] = 4;

        if (curDirection == 0)
        {
            headx -= 1;
        }
        else if (curDirection == 1)
        {
            heady -= 1;
        }
        else if (curDirection == 2)
        {
            headx += 1;
        }
        else
        {
            heady += 1;
        }

        if (map[heady][headx] == 1)
        { // 벽
            return false;
        }

        // 일단 게이트가 무조건 존재한다고 가정.
        // 게이트가 없는 경우에도 실행한다면 if문에 게이트 존재여부 검증 조건 추가 필요
        if (map[heady][headx] == 7)
        {
            int diffx = abs(gate[0][1] - gate[1][1]);
            int diffy = abs(gate[0][0] - gate[1][0]);
            headx = (headx < diffx) ? headx + diffx : headx - diffx;
            heady = (heady < diffy) ? heady + diffy : heady - diffy;
            if (headx == 0)
                curDirection = EAST;
            if (headx == 20)
                curDirection = WEST;
            if (heady == 0)
                curDirection = SOUTH;
            if (heady == 20)
                curDirection = NORTH;
        }

        map[heady][headx] = 3;
        body.push({ heady, headx });
        return true;
    }
};

class growItem
{
    int x, y;
    static int cnt;
    int time = 0;

    // void timeout(){

    // }
};

class poisonItem
{
    int x, y;
    static int cnt;
    int time = 0;

    // void timeout(){
    //     if(time>=5){

    //     }
    // }
};

void map_init();

int kbhit(void)
{
    int ch = getch();
    if (ch != ERR)
    {
        ungetch(ch);
        return 1;
    }
    else
    {
        return 0;
    }
}

void map_random_color()
{
    srand(time(NULL));

    int x = rand();
    int y = rand();
    x %= MAP_SIZE;
    y %= MAP_SIZE;
    while (map[x][y] != 0)
    {
        x = rand();
        y = rand();
        x %= MAP_SIZE;
        y %= MAP_SIZE;
    }
    // cout << x << ' ' << y <<'\n';
    map[x][y] = 1;
}

// 0.5초 sleep이 아닌 다른 구현 필요
void sleep(float seconds)
{
    clock_t startClock = clock();
    float secondsAhead = seconds * CLOCKS_PER_SEC;
    // do nothing until the elapsed time has passed.
    while (clock() < startClock + secondsAhead)
        ;
    return;
}

void draw_snakewindow(WINDOW *snake_win);

// set gate position randomly
void setPosition(int &x, int &y)
{
    int xORy = (rand() % 2);
    int zeroOr20 = (rand() % 2);
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

int main()
{
    WINDOW *snake_win;
    WINDOW *point_win;
    WINDOW* mission_win;

    Snake sk = Snake(MAP_SIZE);

    //---------- set gate in random position ----------
    srand(time(NULL));
    int x1, y1, x2, y2;
    x1 = y1 = x2 = y2 = 0;
    while (x1 == x2 and y1 == y2)
    {
        setPosition(x1, y1);
        setPosition(x2, y2);
    };
    sk.gate[0][0] = y1;
    sk.gate[0][1] = x1;
    sk.gate[1][0] = y2;
    sk.gate[1][1] = x2;
    //---------------- end set gate ----------------

    setlocale(LC_ALL, "");
    map_init();

    initscr();
    resize_term(60, 100);

    start_color();
    init_pair(1, COLOR_RED, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_BLUE, COLOR_CYAN);
    init_pair(4, COLOR_GREEN, COLOR_MAGENTA);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);

    border('*', '*', '*', '*', '*', '*', '*', '*');
    mvprintw(1, 1, "SNAKE GAME");
    refresh();

    //디스플레이 하드코딩 된 부분 수정 필요
    snake_win = newwin(30, 60, 3, 3);
    wbkgd(snake_win, COLOR_PAIR(5));
    wattron(snake_win, COLOR_PAIR(5));
    mvwprintw(snake_win, 1, 1, "Snake_game window");
    wborder(snake_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(snake_win);

    point_win = newwin(15, 29, 3, 64);
    wbkgd(point_win, COLOR_PAIR(5));
    wattron(point_win, COLOR_PAIR(5));
    mvwprintw(point_win, 1, 1, "Score_board");
    wborder(point_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(point_win);


    mission_win = newwin(15, 29, 18, 64);
    wbkgd(mission_win, COLOR_PAIR(5));
    wattron(mission_win, COLOR_PAIR(5));
    mvwprintw(mission_win, 1, 1, "Mission");
    wborder(mission_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(mission_win);

    int key;
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    // 0.5초마다 map 반영해서 그리기
    //여기가 좀 복잡한 문제
    //쓰레드를 따로 둬서 0.5초 동안 입력을 받을 수 있게끔 정밀한 구현이 필요함
    // getch()를 받기 까지 blocking되는 이슈
    // select?
    //키입력 -> 방향전환 -> 움직임

    clock_t startClock;
    clock_t endClock;

    startClock = clock();
    while (1)
    {
        // view gate
        map[sk.gate[0][0]][sk.gate[0][1]] = 7;
        map[sk.gate[1][0]][sk.gate[1][1]] = 7;
        if (kbhit())
        {
            key = getch();
            if (key == 259)
            {
                // printw("up");
                sk.turnDirection(NORTH);
            }
            else if (key == 260)
            {
                // printw("left");
                sk.turnDirection(WEST);
            }
            else if (key == 258)
            {
                // printw("down");
                sk.turnDirection(SOUTH);
            }
            else if (key == 261)
            {
                // printw("right");
                sk.turnDirection(EAST);
            }
            refresh();
        }
        endClock = clock();

        if ((float)(endClock - startClock) / CLOCKS_PER_SEC >= 0.4)
        {
            if (!sk.move())
            {
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

void map_init()
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
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

void draw_snakewindow(WINDOW* snake_win) {
    for (int i = 3; i < MAP_SIZE + 3; i++) {
        int j = 3;
        for (; j < MAP_SIZE + 3; j++) {
            if (map[i - 3][j - 3] == 0) { //white(빈칸)
                mvwprintw(snake_win, i, j * 2, "\u2B1C");
            }
            else if (map[i - 3][j - 3] == 1 || map[i - 3][j - 3] == 2) { //wall
                mvwprintw(snake_win, i, j * 2, "\u2B1B");
            }
            else if (map[i - 3][j - 3] == 3) {  //head of snake
                mvwprintw(snake_win, i, j * 2, "🟨");
            }
            else if (map[i - 3][j - 3] == 4) {  //head of snake
                mvwprintw(snake_win, i, j * 2, "🟧");
            }
            else if (map[i - 3][j - 3] == 5) {  // growth item
                mvwprintw(snake_win, i, j * 2, "🟩");
            }
            else if (map[i - 3][j - 3] == 6) {  //posion item
                mvwprintw(snake_win, i, j * 2, "🟥");
            }
            else if (map[i - 3][j - 3] == 7) {  //Gate
                mvwprintw(snake_win, i, j * 2, "🟪");
            }
        }
        mvwprintw(snake_win, i, j * 2, "\n");
    }
}
