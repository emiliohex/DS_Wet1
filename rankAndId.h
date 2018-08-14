
#ifndef DS_WET1_RANKANDID_H
#define DS_WET1_RANKANDID_H

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

    void setWorkerId(int workerId) {
        this->workerId = workerId;
    }

    void setRank(int rank) {
        this->rank = rank;
    }
};


#endif //DS_WET1_RANKANDID_H
