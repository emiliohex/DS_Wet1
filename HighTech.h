#ifndef HIGHTECH_H_
#define HIGHTECH_H_

#include "library1.h"
#include "AvlTree.h"
#include "Company.h"
#include "Worker.h"
#include "compare.h"

#define CompaniesTree_t AvlTree<int, Company*, IntComparer>
#define AllWorkersTree_t AvlTree<int, Worker*, rankThanId>

#define CompaniesTreeIterator CompaniesTree_t::Iterator
#define AllWorkersTreeIterator AllWorkersTree_t::Iterator

class HighTech {
private:
    CompaniesTree_t* Companies;
    //WorkersTree_t* Workers;
    AllWorkersTree_t* AllWorkersTree;

    Worker* bestWorker;

	void getTree(AllWorkersTree_t* AllWorkersTree, int *workers);
	void deleteWorkerTree(AllWorkersTree_t* AllWorkersTree);
	void deleteCompanyTree(CompaniesTree_t* Companies);
	void printTree(AllWorkersTree_t* PostTree);


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
