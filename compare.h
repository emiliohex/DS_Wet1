#ifndef COMPARE_H
#define COMPARE_H

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
    int operator()(rankAndId* a, rankAndId* b) {
        if(a->getRank() == b->getRank()) {
            if(a->getWorkerId() == b->getWorkerId()) {
                return 0;
            }
            else if (a->getWorkerId() > b->getWorkerId()) {
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

#endif
