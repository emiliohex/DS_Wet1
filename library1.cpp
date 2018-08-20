#include "library1.h"
#include "HighTech.h"

void* init(){
    try{
        HighTech* DS = new HighTech();
        return (void*)DS;
    }
    catch (std::bad_alloc& e) {
        return NULL;
    }
}
StatusType addWorker(void *DS, int workerID, int rank){
    if(DS==NULL) return INVALID_INPUT;
    return ((HighTech*)DS)->addWorker (workerID,rank);
}
StatusType addCompany(void *DS, int companyID){
    if(DS==NULL) return INVALID_INPUT;
    return ((HighTech*)DS)->addCompany (companyID);
}
StatusType addworkerToCompany(void *DS, int workerID, int companyID){
    if(DS==NULL) return INVALID_INPUT;
    return ((HighTech*)DS)->addworkerToCompany (workerID,companyID);
}
StatusType removeWorker(void *DS, int workerID){
    if(DS==NULL) return INVALID_INPUT;
    return ((HighTech*)DS)->removeWorker (workerID);
}
StatusType mergeCompanies(void *DS, int companyID1, int companyID2, int minimalRank){
    if(DS==NULL) return INVALID_INPUT;
    return ((HighTech*)DS)->mergeCompanies (companyID1,companyID2,minimalRank);
}
StatusType changeRank(void *DS, int workerID, int newRank){
    if(DS==NULL) return INVALID_INPUT;
    return ((HighTech*)DS)->changeRank (workerID,newRank);
}
StatusType getBestWorker(void *DS, int companyID, int *workerID){
    if(DS==NULL) return INVALID_INPUT;
    return ((HighTech*)DS)->getBestWorker (companyID,workerID);
}
StatusType getCompanyWorkersByRank (void *DS, int companyID, int **workers, int *numOfWorkers){
    if(DS==NULL) return INVALID_INPUT;
    return ((HighTech*)DS)->getCompanyWorkersByRank (companyID,workers,numOfWorkers);
}
void quit(void** DS){
    if(!DS || !(*DS)){
        return;
    }
    HighTech** highTech = (HighTech**)DS;
    //((HighTech*)*DS)->Quit();
    delete *highTech;
	*DS = NULL;
}

