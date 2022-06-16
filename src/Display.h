#include <ncurses.h>
#include "Snake_map.h"
#include "Point.h"
#include "Mission.h"

void draw_snakewindow(WINDOW* snake_win);
void colorSetting();

void updatePoint(WINDOW* point_win, Point* p);
void updateMission(WINDOW* mission_win, Mission* m, Point* p);

//stage notice
void displayNext(WINDOW* notice_win);
void displayGameClear(WINDOW* notice_win);
void displayDead(WINDOW* notice_win);