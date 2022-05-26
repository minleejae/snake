#include <iostream>
#include "Snake_map.h"
#define MAP_SIZE 21

void map_random_color(int map[][MAP_SIZE])
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


void map_init(int map[][MAP_SIZE])
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



void draw_snakewindow(WINDOW* snake_win, int map[][MAP_SIZE]) {
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
        }
        mvwprintw(snake_win, i, j * 2, "\n");
    }
}