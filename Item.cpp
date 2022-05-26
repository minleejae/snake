#include <iostream>
#include "Item.h"
using namespace std;

Item::Item()
{
    grow_x = 0;
    grow_y = 0;
    poison_x = 20;
    poison_y = 20;
}

// snake의 바디값 설정
void Item::setBody(deque<pair<int, int>> snake, int length)
{
    bodyLength = length;
    for (int i = 0; i < bodyLength; i++)
    {
        pair<int, int> top = snake.front();
        body[i][0] = top.first;
        body[i][1] = top.second;
        // snake.pop_front();
    }
}

// 바디값이 아닌 위치를 범위 내에서 랜덤 설정(1~19)
// posisonItem이랑 위치 겹치면 안되기 때문에 main에서 비교 후 다시 설정해주는 작업해야됨
pair<int, int> Item::getGrowItemPosition()
{
    srand(time(NULL));
    while (true)
    {
        int y = rand() % 20;
        int x = rand() % 20;
        if (positionIsSuited(y, x) && (y != poison_y && x != poison_x))
        {
            grow_y = y;
            grow_x = x;
            return make_pair(y, x);
        }
    }
}

pair<int, int> Item::getPoisonItemPosition()
{
    srand(time(NULL));
    while (true)
    {
        int y = rand() % 20;
        int x = rand() % 20;
        if (positionIsSuited(y, x) && (y != grow_y && x != grow_x))
        {
            poison_y = y;
            poison_x = x;
            return make_pair(y, x);
        }
    }
}

bool Item::positionIsSuited(int y, int x)
{
    if (y != 0 && x != 0)
    {
        for (int i = 0; i < bodyLength; i++)
        {
            if (y == body[i][0] || x == body[i][1])
            {
                return false;
            }
        }
        return true;
    }
    return false;
}