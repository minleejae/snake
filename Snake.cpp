#include <iostream>
#include "Item.h"
#include "Snake.h"

using namespace std;

Snake::Snake() {
    curDirection = WEST;
    length = 3;
    heady = MAP_SIZE / 2;
    headx = MAP_SIZE / 2 - 1;
    body.push_back({ MAP_SIZE / 2, MAP_SIZE / 2 + 1 });
    body.push_back({ MAP_SIZE / 2, MAP_SIZE / 2 });
    body.push_back({ MAP_SIZE / 2, MAP_SIZE / 2 - 1 });
}

deque<pair<int, int>> Snake::getBody() {
    return body;
}

int Snake::getLength() {
    return length;
}
//입력된 키를 기준으로 방향전환
//방향전환시 죽는 경우 있음

bool Snake::turnDirection(int key) {
    if (abs(key - curDirection) == 2) return false;
    curDirection = key;
    return true;
}


//현재 방향을 기준으로 움직임
bool Snake::move(Point* p, Item* item) {
    int pop_fronty = body.front().first;
    int pop_frontx = body.front().second;

    map[pop_fronty][pop_frontx] = 0;
    map[heady][headx] = 4;

    //방향에 따른 이동
    if (curDirection == 0) headx -= 1;
    else if (curDirection == 1) heady -= 1;
    else if (curDirection == 2) headx += 1;
    else heady += 1;

    // item 만났을 때 바디값 조절
    if (map[heady][headx] == 5) {
        //point의 growitem 증가 
        p->addGrowItem();
        p->addCurrentLength();
        item->removeItem(heady, headx);


        map[pop_fronty][pop_frontx] = 4;
        length++;// 바디값이 늘어난 것을 명시, item 클래스에 바디 길이를 체크할 때 활용하기 때문에.
    }
    else if (map[heady][headx] == 6) {
        p->addPoisonItem();
        p->minusCurrentLength();
        item->removeItem(heady, headx);

        body.pop_front();
        int body_lasty = body.front().first;
        int body_lastx = body.front().second;
        body.pop_front();
        map[body_lasty][body_lastx] = 0;
        length--;
    }
    else {
        body.pop_front();
    }

    //snake 머리가 벽을 만나거나 자기 몸통을 만나면 게임 끝
    if (map[heady][headx] == 1 || map[heady][headx] == 2 || map[heady][headx] == 4) {
        return false;
    }

    if (map[heady][headx] == 7) {
        p->addUseGate();
        movebyDoor();
    }
    map[heady][headx] = 3;
    body.push_back({ heady,headx });
    return true;
}

void Snake::movebyDoor()
{
    headx = (headx == gate[0][1]) ? gate[1][1] : gate[0][1];
    heady = (heady == gate[0][0]) ? gate[1][0] : gate[0][0];

    // 게이트가 벽에 있을 때 방향 조절
    if (headx == 0)
        curDirection = EAST;
    if (headx == 20)
        curDirection = WEST;
    if (heady == 0)
        curDirection = SOUTH;
    if (heady == 20)
        curDirection = NORTH;

    // 게이트가 맵 가운데에 있을 때
    // 원래 방향이 동쪽일 때
    if (curDirection == EAST && map[heady][headx + 1] == 1)
    {
        if (map[heady + 1][headx] == 1)
        {
            if (map[heady - 1][headx] == 1)
            {
                if (map[heady][headx - 1] == 1)
                {
                    // cout << "SETTING DIRECTION ERROR" << endl;
                }
                else
                {
                    curDirection = WEST;
                }
            }
            else
            {
                curDirection = NORTH;
            }
        }
        else
        {
            curDirection = SOUTH;
        }
    }
    // 원래 방향이 서쪽일 때
    else if (curDirection == WEST && map[heady][headx - 1] == 1)
    {
        if (map[heady - 1][headx] == 1)
        {
            if (map[heady + 1][headx] == 1)
            {
                if (map[heady][headx + 1] == 1)
                {
                    // cout << "SETTING DIRECTION ERROR" << endl;
                }
                else
                {
                    curDirection = EAST;
                }
            }
            else
            {
                curDirection = SOUTH;
            }
        }
        else
        {
            curDirection = NORTH;
        }
    }
    // 원래 방향이 남쪽일 때
    else if (curDirection == SOUTH && map[heady + 1][headx] == 1)
    {
        if (map[heady][headx - 1] == 1)
        {
            if (map[heady][headx + 1] == 1)
            {
                if (map[heady - 1][headx] == 1)
                {
                    // cout << "SETTING DIRECTION ERROR" << endl;
                }
                else
                {
                    curDirection = NORTH;
                }
            }
            else
            {
                curDirection = EAST;
            }
        }
        else
        {
            curDirection = WEST;
        }
    }
    // 원래 방향이 북쪽일 때
    else if (curDirection == NORTH && map[heady - 1][headx] == 1)
    {
        if (map[heady][headx + 1] == 1)
        {
            if (map[heady][headx - 1] == 1)
            {
                if (map[heady + 1][headx] == 1)
                {
                    // cout << "SETTING DIRECTION ERROR" << endl;
                }
                else
                {
                    curDirection = SOUTH;
                }
            }
            else
            {
                curDirection = WEST;
            }
        }
        else
        {
            curDirection = EAST;
        }
    }

    //door 이후로 이동하게끔 추가 
    if (curDirection == EAST) {
        headx++;
    }
    else if (curDirection == WEST) {
        headx--;
    }
    else if (curDirection == SOUTH) {
        heady++;
    }
    else if (curDirection == NORTH) {
        heady--;
    }
}
