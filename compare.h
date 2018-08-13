#ifndef DS_WET1_COMPARE_H
#define DS_WET1_COMPARE_H

#include "Worker.h"


class IntComparer {
public:
    int operator()(int a, int b) {
        if(a == b) {
            return 0;
        }
        if (a > b) {
            return 1;
        }
        return -1;
    }
};


class rankThanId {
public:
    int operator()(Worker* a, Worker* b) {
        if(a->getRank() == b->getRank()) {
            if(a->getId() == b->getId()) {
                return 0;
            }
            else if (a->getId() > b->getId()) {
                return -1; // the post with the smaller post id should be greater
            }

            return 1;
        }
        else if (a->getRank() > b->getRank()) {
            return 1;
        }
        return -1;
    }
};

#endif //DS_WET1_COMPARE_H
