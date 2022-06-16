#include <iostream>
#ifndef __SNAKE_MAP__
#define __SNAKE_MAP__
#define MAP_SIZE 21
#define GATE_SIZE 2
        //     0        1         2        3           4           5            6         7
enum color {WHITE, IMMUNE_WALL, WALL, SNAKE_HEAD, SNAKE_BODY, GROWTH_ITEM, POISON_ITEM, GATE};

extern int stage;
extern bool waitGate;
extern int forGate;
extern int gate[][GATE_SIZE];
extern int map[MAP_SIZE][MAP_SIZE];
extern int map1[][GATE_SIZE];
extern int map2[][GATE_SIZE];
extern int map3[][GATE_SIZE];
extern int map4[][GATE_SIZE];
extern int walls[][GATE_SIZE];

void map_init(int stage);
void findImmuneWall();
void makeGate();

#endif
