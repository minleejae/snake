#include <iostream>
#include <queue>
#define MAP_SIZE 21

#ifndef __SNAKE__
#define __SNAKE__


enum direction {WEST,NORTH,EAST, SOUTH}; //서: 0 북:1 동:2 남:3

class Snake{
public:
// private:
    int curDirection = WEST;
    int headx, heady;
    int length = 3;
    std::deque<std::pair<int, int>> body; // (y,x)
// public:
    // 게이트는 하나만 존재하면 된다.  snake 객체에 정의
    int gate[2][2] = { { 0, 0 }, { 0, 0 } }; // [ [y1, x1], [y2, x2] ]
    // 게이트 랜덤생성을 위한 카운트
    int waitGate = rand() % 10 + 10;

    void setPosition(int &x, int &y);

    void makeGate();

    deque<pair<int, int>> getBody();
    int getLength();

    Snake(int map_size);
    //입력된 키를 기준으로 방향전환
    //방향전환시 죽는 경우 있음
    bool turnDirection(int key);
    //현재 방향을 기준으로 움직임
    bool move(int map[][MAP_SIZE]);
};


#endif