#include "Item.h"
using namespace std;

Item::Item() {
    total_item = 0;
}

void Item::produceItem() {
    if (total_item < 3) {
        //현재 총 아이템이 3개 미만이라면 랜덤으로 poision이나 growth중 새로운 아이템 생성
        int newItem = (rand() % 2) + 5;
        
        //item이 언제생성되었는지 기록
        clock_t now = clock();

        int y = rand() % MAP_SIZE;
        int x = rand() % MAP_SIZE;
        if (map[y][x] == 0) {
            map[y][x] = newItem;
            total_item++;

            for (int i = 0; i < 3; i++) {
                //remove에서 y값을 -1로 만들어준다.
                if (iv[i].y == -1) {
                    iv[i].y = y;
                    iv[i].x = x;
                    iv[i].time = now;
                    break;
                }
            }
        }
    }
}

void Item::addTotalItem() {
    total_item++;
}

void Item::minusTotalItem() {
    total_item--;
}

//시간이 지나서 없어지는 경우
void Item::removeItem() {
    if (total_item > 0) {
        clock_t now = clock();
        for (int i = 0; i < 3; i++) {
            if ((float(now - iv[i].time) / CLOCKS_PER_SEC > 5) && (iv[i].y > 0)) {
                map[iv[i].y][iv[i].x] = 0;
                iv[i].y = -1;
                total_item--;
            }
        }
    }
}

//Snake가 먹어서 없어지는 경우
void Item::removeItem(int y, int x) {
    if (total_item > 0) {
        for (int i = 0; i < 3; i++) {
            if (iv[i].y == y && iv[i].x == x) {
                iv[i].y = -1;
                total_item--;
            }
        }
    }
}