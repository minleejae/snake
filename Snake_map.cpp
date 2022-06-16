#include <iostream>
#include "Snake_map.h"

int stage = 0;
bool waitGate = 1;
int forGate = 10;
int gate[2][GATE_SIZE] = { { 0, 0 }, { 0, 0 } };
int map[MAP_SIZE][MAP_SIZE];

void map_init(int stage) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            map[i][j] = 0;
        }
    }

    for (int i = 0; i < MAP_SIZE; i++) {
        map[i][MAP_SIZE - 1] = WALL;
        map[i][0] = WALL;
        map[MAP_SIZE - 1][i] = WALL;
        map[0][i] = WALL;
    }

    map[MAP_SIZE / 2][MAP_SIZE / 2] = SNAKE_BODY;
    map[MAP_SIZE / 2][MAP_SIZE / 2 + 1] = SNAKE_BODY;
    map[MAP_SIZE / 2][MAP_SIZE / 2 - 1] = SNAKE_BODY;


    //stage별로 wall 다르게
    if (stage == 1) {
        for (int i = 0; i < 10; i++) {
            map[8][i] = 2;
        }
    }
    else if (stage == 2) {
        for (int i = 3; i < 14; i++) {
            map[5][i] = 2;
        }

        for (int i = 3; i < 14; i++) {
            map[i][3] = 2;
        }
    }
    else if (stage == 3) {
        for (int i = 4; i < 10; i++) {
            map[i][i] = 2;
        }
    }
    else if (stage == 4) {
        for (int i = 4; i < 10; i++) {
            map[i][i] = 2;
        }
        for (int i = 4; i < 10; i++) {
            map[10 - i][i] = 2;
        }

        for (int i = 19; i >= 12; i--) {
            map[i][i] = 2;
        }
        for (int i = 19; i >= 15; i--) {
            map[21 - i][i] = 2;
        }
    }

    //immune wall 적용
    findImmuneWall();
}

//wall과 gate가 등장하면 안되는 ImmuneWall 구분
void findImmuneWall() {
    int dy[4] = { 0,1,0,-1 };
    int dx[4] = { 1,0,-1,0 };

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (map[i][j] == 2) {

                int cnt = 0;
                for (int k = 0; k < 4; k++) {
                    int ny = i + dy[k];
                    int nx = j + dx[k];

                    if (ny < 0 || ny >= MAP_SIZE || nx < 0 || nx >= MAP_SIZE) continue;
                    if (map[ny][nx] != 1 && map[ny][nx] != 2) cnt++;
                }

                //사방이 wall인 immune wall
                if (cnt == 0) map[i][j] = 1;
            }
        }
    }
}


// immune wall중에서 Gate 생성
void makeGate() {
    int x1 = rand() % MAP_SIZE;
    int y1 = rand() % MAP_SIZE;

    int x2 = rand() % MAP_SIZE;
    int y2 = rand() % MAP_SIZE;

    while (map[y1][x1] != 2) {
        x1 = rand() % MAP_SIZE;
        y1 = rand() % MAP_SIZE;
    }

    map[y1][x1] = GATE;

    while (map[y2][x2] != 2) {
        x2 = rand() % MAP_SIZE;
        y2 = rand() % MAP_SIZE;
    }

    map[y2][x2] = GATE;

    gate[0][0] = y1;
    gate[0][1] = x1;
    gate[1][0] = y2;
    gate[1][1] = x2;
}
