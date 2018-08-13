#ifndef COMPANY_H_
#define COMPANY_H_

#include "AvlTree.h"
#include "Worker.h"

#define WorkersTree_t AvlTree<int, Worker*, IntComparer>
#define WorkersTreeIterator WorkersTree_t::Iterator


class Company {
private:
	int companyId;
    Worker* bestWorker;

public:
	WorkersTree_t* workers;

	int getId() {
		return this->companyId;
	}

	Worker* getBestWorker() {
		return this->bestWorker;
	}

	void setBestWorker(Worker* bestWorker) {
		this->bestWorker = bestWorker;
	}

	Company(int companyId) {
		this->companyId = companyId;
		workers = new WorkersTree_t();
		bestWorker = NULL;
	}

	~Company() {
		WorkersTreeIterator = workers->GetIterator();
		while (*WorkersTreeIterator != NULL) {
			delete(WorkersTreeIterator.key());
			WorkersTreeIterator++;
		}

		delete(workers);
	}
};



#endif
