#include "Display.h"

void draw_snakewindow(WINDOW* snake_win) {
    for (int i = 4; i < MAP_SIZE + 4; i++) {
        for (int j = 4; j < MAP_SIZE + 4; j++) {
            if (map[i - 4][j - 4] == 0) { //white(빈칸)
                wattron(snake_win, COLOR_PAIR(1));
                mvwprintw(snake_win, i, j * 2, "  ");
                // attroff(COLOR_PAIR(1));
            }
            else if (map[i - 4][j - 4] == 1) { //Immune wall
                wattron(snake_win, COLOR_PAIR(2));
                mvwprintw(snake_win, i, j * 2, "  ");
                // attroff(COLOR_PAIR(2));
            }
            else if (map[i - 4][j - 4] == WALL) { //wall
                wattron(snake_win, COLOR_PAIR(12));
                mvwprintw(snake_win, i, j * 2, "  ");
                // attroff(COLOR_PAIR(2));
            }
            else if (map[i - 4][j - 4] == SNAKE_HEAD) {  //head of snake
                wattron(snake_win, COLOR_PAIR(SNAKE_HEAD));
                mvwprintw(snake_win, i, j * 2, "  ");
                // attroff(COLOR_PAIR(3));
            }
            else if (map[i - 4][j - 4] == SNAKE_BODY) {  //body of snake
                wattron(snake_win, COLOR_PAIR(SNAKE_BODY));
                mvwprintw(snake_win, i, j * 2, "  ");
            }
            else if (map[i - 4][j - 4] == GROWTH_ITEM) {  // growth item
                wattron(snake_win, COLOR_PAIR(GROWTH_ITEM));
                mvwprintw(snake_win, i, j * 2, "  ");
            }
            else if (map[i - 4][j - 4] == POISON_ITEM) {  //posion item
                wattron(snake_win, COLOR_PAIR(POISON_ITEM));
                mvwprintw(snake_win, i, j * 2, "  ");
            }
            else if (map[i - 4][j - 4] == GATE) {  //Gate
                wattron(snake_win, COLOR_PAIR(GATE));
                mvwprintw(snake_win, i, j * 2, "  ");
            }
        }
    }
    wrefresh(snake_win);
}



void colorSetting() {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(4, COLOR_CYAN, COLOR_CYAN);
    init_pair(5, COLOR_GREEN, COLOR_GREEN);
    init_pair(6, COLOR_RED, COLOR_RED);
    init_pair(7, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(8, COLOR_BLACK, COLOR_WHITE);
    init_pair(9, COLOR_BLACK, COLOR_YELLOW);
    init_pair(10, COLOR_BLACK, COLOR_GREEN);
    init_pair(11, COLOR_BLACK, COLOR_RED);
    init_color(COLOR_BLUE, 500, 500, 500);
    init_pair(12, COLOR_BLUE, COLOR_BLUE);

}




void updatePoint(WINDOW* point_win, Point* p) {
    //이미 써져있는 것 초기화
    for (int i = 2; i <= 5; i++) {
        mvwprintw(point_win, i, 1, "                  ");
    }
    p->updateMaxLength();
    mvwprintw(point_win, 2, 1, "B: %d / %d", p->getCurrentLength(), p->getMaxLength());
    mvwprintw(point_win, 3, 1, "+: %d", p->getGrowItem());
    mvwprintw(point_win, 4, 1, "-: %d", p->getPoisonItem());
    mvwprintw(point_win, 5, 1, "G: %d", p->getUseGate());
    wrefresh(point_win);
}



void updateMission(WINDOW* mission_win, Mission* m, Point* p) {
    for (int i = 2; i <= 5; i++) {
        mvwprintw(mission_win, i, 1, "                  ");
    }
    //현재 Point와 비교해서 mission 달성 여부 check하기
    m->updateAll(p);

    mvwprintw(mission_win, 2, 1, "B: %d (%c)", m->getB(), (m->getBflag() ? 'V' : ' '));
    mvwprintw(mission_win, 3, 1, "+: %d (%c)", m->getGrowItem(), (m->getGrowItemFlag() ? 'V' : ' '));
    mvwprintw(mission_win, 4, 1, "-: %d (%c)", m->getPoisonItem(), (m->getPoisonItemFlag() ? 'V' : ' '));
    mvwprintw(mission_win, 5, 1, "G: %d (%c)", m->getUseGate(), (m->getUseGateFlag() ? 'V' : ' '));
    wrefresh(mission_win);
}

void time_wait(float time){
    clock_t startClock = clock();
    clock_t stageWait = clock();
    while ((float)(stageWait - startClock) / CLOCKS_PER_SEC < time) {
        stageWait = clock();
    }
}


void displayDead(WINDOW* notice_win) {
    notice_win = newwin(8, 20, 15,22);
    wbkgd(notice_win, COLOR_PAIR(11));
    wattron(notice_win, COLOR_PAIR(11));
    mvwprintw(notice_win, 3, 3, "Snake is Dead!");
    mvwprintw(notice_win, 4, 5, "Game End!");
    wborder(notice_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(notice_win);
    time_wait(3);
    delwin(notice_win);
}

void displayNext(WINDOW* notice_win) {
    notice_win = newwin(8, 20, 15,22);
    wbkgd(notice_win, COLOR_PAIR(7));
    wattron(notice_win, COLOR_PAIR(9));
    mvwprintw(notice_win, 3, 5, "Next_Stage!");
    mvwprintw(notice_win, 4, 3, "Press Any Key!");
    wborder(notice_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(notice_win);
    time_wait(2);
    delwin(notice_win);
}

void displayGameClear(WINDOW* notice_win) {
    notice_win = newwin(8, 20, 15,22);
    wbkgd(notice_win, COLOR_PAIR(10));
    wattron(notice_win, COLOR_PAIR(10));
    mvwprintw(notice_win, 2, 2, "Congratulations!");
    mvwprintw(notice_win, 3, 2, "ALL Stage Clear!");
    mvwprintw(notice_win, 4, 4, "Game End!");
    wborder(notice_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(notice_win);
    time_wait(3);
    delwin(notice_win);
}