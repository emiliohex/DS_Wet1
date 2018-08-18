#ifndef COMPARE_H
#define COMPARE_H

#include "Worker.h"


class IntComparer {
public:
    int operator()(int a, int b) {
        return (a<b);
    }
};


class rankThanId {
public:
    bool operator()(rankAndId* a, rankAndId* b) {
        return (b->getRank() > a->getRank() || (b->getRank() == a->getRank() && b->getWorkerId() < a->getWorkerId()));
    }
};

#endif
