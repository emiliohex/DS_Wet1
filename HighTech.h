#ifndef HIGHTECH_H_
#define HIGHTECH_H_

#include "library1.h"
#include "AvlTree.h"
#include "Company.h"
#include "Worker.h"
#include "compare.h"
#include "rankAndId.h"
#include "math.h"

#define CompaniesTree_t AvlTree<int, Company*, IntComparer>
#define AllWorkersTreeId_t AvlTree<int, Worker*, IntComparer>
#define AllWorkersTreeRank_t AvlTree<rankAndId* , Worker*, rankThanId>

#define AllWorkersTreeIterator AllWorkersTreeRank_t::iterator

class HighTech {
private:
    CompaniesTree_t* Companies;
	AllWorkersTreeId_t* Workers;
	AllWorkersTreeRank_t* AllWorkersTree;

    Worker* bestWorker;

	void getTree(AllWorkersTreeRank_t* AllWorkersTree, int *workers,int size);
	void merge(Worker* a[], int m, Worker* b[], int n, Worker* sorted[]);
	void reverse(int* start, int* end);


public:
	HighTech();
	~HighTech(){}
	StatusType addWorker(int workerID, int rank);
    StatusType addCompany(int companyID);
    StatusType addworkerToCompany(int workerID, int companyID);
    StatusType removeWorker(int workerID);
    StatusType mergeCompanies(int companyID1, int companyID2, int minimalRank);
    StatusType changeRank(int workerID, int newRank);
    StatusType getBestWorker(int companyID, int *workerID);
    StatusType getCompanyWorkersByRank (int companyID, int **workers, int *numOfWorkers);
	void Quit();
};
#endif
