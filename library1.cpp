#include "library1.h"
#include "HighTech.h"

void* init(){
    HighTech * DS = new HighTech();
	return (void*)DS;
}
StatusType addWorker(void *DS, int workerID, int rank){
    return ((HighTech*)DS)->addWorker (workerID,rank);
}
StatusType addCompany(void *DS, int companyID){
    return ((HighTech*)DS)->addCompany (companyID);
}
StatusType addworkerToCompany(void *DS, int workerID, int companyID){
    return ((HighTech*)DS)->addworkerToCompany (workerID,companyID);
}
StatusType removeWorker(void *DS, int workerID){
    return ((HighTech*)DS)->removeWorker (workerID);
}
StatusType mergeCompanies(void *DS, int companyID1, int companyID2, int minimalRank){
    return ((HighTech*)DS)->mergeCompanies (companyID1,companyID2,minimalRank);
}
StatusType changeRank(void *DS, int workerID, int newRank){
    return ((HighTech*)DS)->changeRank (workerID,newRank);
}
StatusType getBestWorker(void *DS, int companyID, int *workerID){
    return ((HighTech*)DS)->getBestWorker (companyID,workerID);
}
StatusType getCompanyWorkersByRank (void *DS, int companyID, int **workers, int *numOfWorkers){
    return ((HighTech*)DS)->getCompanyWorkersByRank (companyID,workers,numOfWorkers);
}
void quit(void** DS){
    ((HighTech*)*DS)->Quit();
	delete((HighTech*)*DS);
	(*DS) = NULL;
}

