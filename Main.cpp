#include <cstdlib>
#include <ctime>
#include <iostream>
#include <locale.h>
#include <ncurses.h>
#include "Snake.h"
#include "Snake_map.h"
#include "Item.h"
#define MAP_SIZE 21

// 해야될 것
// 다양한 맵 만들기
// item 먹는 것
// 차원의 문(?) -> 이동까지 구현필요
// 게임 종료시
// 포인트와 미션 구현

using namespace std;

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
void map_init();

const char* returnScore(int score){
    string tmp = to_string(score);
    char const *result = tmp.c_str();
    return result;
}

int main()
{
    WINDOW *snake_win;
    WINDOW *point_win;
    WINDOW *mission_win;

    Snake sk = Snake(MAP_SIZE);
    srand(time(NULL));

    setlocale(LC_ALL, "");
    map_init(map);

    initscr();
    resize_term(35, 100);

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

    int scoreInteger = 0;
    string tmp = to_string(scoreInteger);
    char const *scoreChar = tmp.c_str();

    point_win = newwin(15, 29, 3, 64);
    wbkgd(point_win, COLOR_PAIR(1));
    wattron(point_win, COLOR_PAIR(8));
    mvwprintw(point_win, 1, 1, "Score_board");

    mvwprintw(point_win, 5, 3, "Score : ");
    mvwprintw(point_win, 5, 11, scoreChar);
    
    wborder(point_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(point_win);


    mission_win = newwin(15, 29, 18, 64);
    wbkgd(mission_win, COLOR_PAIR(1));
    wattron(mission_win, COLOR_PAIR(8));
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

    Item item = Item();
    item.setBody(sk.getBody(), sk.getLength());

    pair<int, int> growitem = item.getGrowItemPosition();
    map[growitem.first][growitem.second] = 5;

    pair<int, int> poisonitem = item.getPoisonItemPosition();
    map[poisonitem.first][poisonitem.second] = 6;
    
    int countSecond = 0;

    while (1)
    {
        // view gate
        if ((sk.gate[0][0] + sk.gate[0][1] + sk.gate[1][0] + sk.gate[0][1]) > 0)
        {
            map[sk.gate[0][0]][sk.gate[0][1]] = 7;
            map[sk.gate[1][0]][sk.gate[1][1]] = 7;
        }
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

        if (growitem.first == sk.heady && growitem.second == sk.headx) {
            map[growitem.first][growitem.second] = 0;
            item.setBody(sk.getBody(), sk.getLength());
            growitem = item.getGrowItemPosition();
            map[growitem.first][growitem.second] = 5;

            
            scoreChar = returnScore(++scoreInteger);
            mvwprintw(point_win, 5, 11, scoreChar);
            wrefresh(point_win);
        }

        // 포인즌 아이템이 안뜸 초기에 뜨다가 말음. 
        if (poisonitem.first == sk.heady && poisonitem.second == sk.headx) {
            map[poisonitem.first][poisonitem.second] = 0;
            item.setBody(sk.getBody(), sk.getLength());
            poisonitem = item.getPoisonItemPosition();
            map[poisonitem.first][poisonitem.second] = 6;
        }

        if (countSecond == 10){
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

        if ((float)(endClock - startClock) / CLOCKS_PER_SEC >= 0.4)
        {
            if (!sk.move(map))
            {
                break;
            };
            startClock = endClock;
            draw_snakewindow(snake_win, map);
            wrefresh(snake_win);
        }
    }

    // wborder(snake_win, '|','|','-','-','+','+','+','+');
    getch();
    delwin(snake_win);
    endwin();
    return 0;
}

