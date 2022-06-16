#include "keyboardEventhandler.h"

int kbhit() {
    int ch = getch();
    if (ch != ERR) {
        ungetch(ch);
        return 1;
    }
    else {
        return 0;
    }
}

bool keyEventHandler(Snake& sk) {
    bool flag = true;
    if (kbhit()) {
        int key = getch();
        if (key == 259) {
            // printw("up");
            flag = sk.turnDirection(NORTH);
        }
        else if (key == 260) {
            // printw("left");
            flag = sk.turnDirection(WEST);
        }
        else if (key == 258) {
            // printw("down");
            flag = sk.turnDirection(SOUTH);
        }
        else if (key == 261) {
            // printw("right");
            flag = sk.turnDirection(EAST);
        }
    }

    return flag;
}