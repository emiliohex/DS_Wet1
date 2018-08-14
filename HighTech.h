#ifndef HIGHTECH_H_
#define HIGHTECH_H_

#include "library1.h"
#include "AvlTree.h"
#include "Company.h"
#include "Worker.h"
#include "compare.h"
#include "rankAndId.h"

#define CompaniesTree_t AvlTree<int, Company*, IntComparer>
#define AllWorkersTreeId_t AvlTree<int, Worker*, IntComparer>
#define AllWorkersTreeRank_t AvlTree<rankAndId* , Worker*, rankThanId>

#define CompaniesTreeIterator CompaniesTree_t::Iterator
#define AllWorkersTreeIterator AllWorkersTreeRank_t::Iterator

class HighTech {
private:
    CompaniesTree_t* Companies;
	AllWorkersTreeId_t* Workers;
	AllWorkersTreeRank_t* AllWorkersTree;

    Worker* bestWorker;

	void getTree(AllWorkersTreeRank_t* AllWorkersTree, int *workers);
	void deleteWorkerTree(AllWorkersTreeRank_t* AllWorkersTree);
	void deleteCompanyTree(CompaniesTree_t* Companies);
	void printTree(AllWorkersTreeRank_t* PostTree);


public:
	HighTech();
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
