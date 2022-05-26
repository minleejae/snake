#include <iostream>
#include <deque>
using namespace std;

#ifndef __Item__
#define __Item__

class Item {

    int grow_x, grow_y, poison_x, poison_y;
    int cnt;
    int bodyLength;
    int body[][2];

public:
    void setBody(deque<pair<int, int>> snake, int length);
    pair<int, int> getGrowItemPosition();
    pair<int, int> getPoisonItemPosition();
    bool positionIsSuited(int y, int x);
};

#endif