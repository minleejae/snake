#include <iostream>
#include "Snake.h"
#define MAP_SIZE 21

using namespace std;

Snake::Snake(int map_size)
{
    heady = map_size / 2;
    headx = map_size / 2 - 1;
    body.push_back({map_size / 2, map_size / 2 + 1});
    body.push_back({map_size / 2, map_size / 2});
    body.push_back({map_size / 2, map_size / 2 - 1});
}

deque<pair<int, int>> Snake::getBody()
{
    return body;
}

int Snake::getLength()
{
    return length;
}

//입력된 키를 기준으로 방향전환
//방향전환시 죽는 경우 있음
bool Snake::turnDirection(int key)
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
// set gate position randomly
void Snake::setPosition(int &x, int &y)
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
void Snake::makeGate()
{
    int x1, y1, x2, y2;
    x1 = y1 = x2 = y2 = 0;
    while (x1 == x2 and y1 == y2)
    {
        setPosition(x1, y1);
        setPosition(x2, y2);
    };
    gate[0][0] = y1;
    gate[0][1] = x1;
    gate[1][0] = y2;
    gate[1][1] = x2;
}
//현재 방향을 기준으로 움직임

bool Snake::move(int map[][MAP_SIZE])
{
    int popy = body.front().first;
    int popx = body.front().second;
    body.pop_front();
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

    // item 만났을 때 바디값 조절
    if (map[heady][headx] == 5)
    {
        map[popy][popx] = 4;
        length++; // 바디값이 늘어난 것을 명시, item 클래스에 바디 길이를 체크할 때 활용하기 때문에.
    }
    else if (map[heady][headx] == 6)
    {
        body.pop_front();
        int body_lasty = body.front().first;
        int body_lastx = body.front().second;
        body.pop_front();
        map[body_lasty][body_lastx] = 0;
        length--;
    }
    else
    {
        body.pop_front();
    }

    if (map[heady][headx] == 1)
    { // 벽
        return false;
    }

    // count wait gate
    if (waitGate == 0)
    {
        makeGate();
    }
    waitGate--;
    if (map[heady][headx] == 7)
    {
        headx = (headx == gate[0][1]) ? gate[1][1] : gate[0][1];
        heady = (heady == gate[0][0]) ? gate[1][0] : gate[0][0];
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
    body.push_back({heady, headx});
    return true;
}