#include <iostream>
#include <deque>
#include "Snake_map.h"

#ifndef __ITEM__
#define __ITEM__

struct ItemValue{
    int y, x;
    clock_t time;
    ItemValue(){
        y = -1, x= -1, time = clock();
    }
    ItemValue(int y_,int x_,clock_t time_){
        y = y_;
        x = x_;
        time = time_;
    }
};

class Item {
private:
    int total_item;
    ItemValue iv[3];
public:
    Item();
    void addTotalItem();
    void minusTotalItem();

    void produceItem();
    void removeItem();
    void removeItem(int y, int x);
};

#endif
