
#ifndef RANKANDID_H
#define RANKANDID_H

class rankAndId {
private:
    int workerId;
    int rank;

public:
    rankAndId(int workerId, int rank) {
        this->workerId = workerId;
        this->rank = rank;
    }

    int getWorkerId() {
        return this->workerId;
    }

    int getRank() {
        return this->rank;
    }

};


#endif
