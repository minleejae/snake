#include <iostream>
#include <ncurses.h>
#define MAP_SIZE 21
#ifndef __SNAKE_MAP__
#define __SNAKE_MAP__

void map_init(int map[][MAP_SIZE]);

void draw_snakewindow(WINDOW* snake_win, int map[][MAP_SIZE]);

void map_random_color(int map[][MAP_SIZE]);

#endif