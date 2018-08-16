#ifndef WORKER_H_
#define WORKER_H_

#include "rankAndId.h"
#include <stdio.h>
#include <stdlib.h>
class Company;

class Worker {
private:
	int workerId;
	int rank;
	Company* company;
    rankAndId* newRankAndId ;


public:
	Worker(int workerId, int rank) {
		this->workerId = workerId;
		this->rank = rank;
		this->company=NULL;
        this->newRankAndId = new rankAndId(workerId,rank);
    }
	void changeRank(int newRank) {
		this->rank=newRank;
	}
	int getRank() {
		return this->rank;
	}
	int getId() {
		return this->workerId;
	}
	void setCompany(Company* newCompany){
		this->company=newCompany;
	}
	Company* getCompany(){
		return this->company;
	}
    rankAndId* getRankAndId() {
        return newRankAndId;
    }
};

#endif