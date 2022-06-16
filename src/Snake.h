#include <iostream>
#include "Snake_map.h"
#include "Point.h"
#include "Item.h"

#ifndef __SNAKE__
#define __SNAKE__

//snake의 방향
enum direction { WEST, NORTH, EAST, SOUTH }; //서: 0 북:1 동:2 남:3

class Snake {
public:
// private:
    int curDirection;
    int headx, heady;
    int length;
    std::deque<std::pair<int, int> > body; // (y,x)
    Snake();
    std::deque<std::pair<int, int> > getBody();
    int getLength();
    bool turnDirection(int key);
    void movebyDoor();
    //move 함수를 통해 item과 door만났을 때 모두 처리
    bool move(Point *p, Item* item);
};

#endif
