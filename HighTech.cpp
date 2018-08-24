
#include "HighTech.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

HighTech::HighTech(){
    this->Companies=new CompaniesTree_t();
    this->Workers=new AllWorkersTreeId_t();
    this->AllWorkersTree=new AllWorkersTreeRank_t();
    this->bestWorker=NULL;
}
StatusType HighTech::addWorker(int workerID, int rank){
    if(workerID<=0 || rank<=0){
        return INVALID_INPUT;
    }
    if(this->Workers->exists(workerID) == true){
        return FAILURE;
    }
    Worker* newWorker;
    try{
        newWorker=new Worker(workerID,rank);
    } catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
    if(this->Workers->add(workerID, newWorker) == false){
        return ALLOCATION_ERROR;
    }
    rankAndId* newRankAndId = newWorker->getRankAndId();
    if(this->AllWorkersTree->add(newRankAndId, newWorker) == false){
        return ALLOCATION_ERROR;
    }
    if(this->bestWorker==NULL || this->bestWorker->getRank()<rank){
        this->bestWorker=newWorker;
    }
    return SUCCESS;
}
StatusType HighTech::addCompany(int companyID){
    if(companyID<=0){
        return INVALID_INPUT;
    }
    if(this->Companies->exists(companyID) == true){
        return FAILURE;
    }
    Company* newCompany;
    try{
        newCompany=new Company(companyID);
    } catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
    if(this->Companies->add(companyID,newCompany)==false){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;

}
StatusType HighTech::addworkerToCompany(int workerID, int companyID){
    if(workerID<=0 || companyID<=0){
        return INVALID_INPUT;
    }
    Worker* WorkerExists = this->Workers->find(workerID);
    Company* CompanyExists = this->Companies->find(companyID);
    if( WorkerExists==NULL || CompanyExists==NULL){
        return FAILURE;
    }
    rankAndId* newRankAndId = WorkerExists->getRankAndId();
    if(CompanyExists->workers->add(newRankAndId, WorkerExists)==false){
        return ALLOCATION_ERROR;
    }
    if(CompanyExists->getBestWorker()==NULL ||  CompanyExists->getBestWorker()->getRank() < WorkerExists->getRank()){
        CompanyExists->setBestWorker(WorkerExists);
    }
    WorkerExists->setCompany(CompanyExists);
    return SUCCESS;
}

StatusType HighTech::removeWorker(int workerID){
    if(workerID<=0){
        return INVALID_INPUT;
    }
    Worker* WorkerExists = this->Workers->find(workerID);
    if( WorkerExists==NULL){
        return FAILURE;
    }
    rankAndId* newRankAndId = WorkerExists->getRankAndId();
    Company* newCompany = WorkerExists->getCompany();
    WorkerExists->setCompany(NULL);
    if(newCompany!=NULL){
        newCompany->workers->remove(newRankAndId);
        if(newCompany->getBestWorker()== WorkerExists){
            newCompany->setBestWorker(newCompany->workers->getMax());
        }
    }
    if(bestWorker==WorkerExists){
        bestWorker=this->Workers->getMax();
    }
    this->Workers->remove(workerID);
    this->AllWorkersTree->remove(newRankAndId);
    delete(WorkerExists);
    return SUCCESS;
}

StatusType HighTech::mergeCompanies(int companyID1, int companyID2, int minimalRank){
    if(companyID1 <= 0 || companyID2 <= 0 || companyID1 == companyID2 || minimalRank<=0){
        return INVALID_INPUT;
    }

    Company* Company1Exists = this->Companies->find(companyID1);
    Company* Company2Exists = this->Companies->find(companyID2);
    if( Company1Exists==NULL || Company2Exists==NULL){
        return FAILURE;
    }
    int newCompanyId;
    if(Company1Exists->workers->getSize() > Company2Exists->workers->getSize()){
        newCompanyId=Company1Exists->getId();
    }
    if(Company1Exists->workers->getSize() < Company2Exists->workers->getSize()){
        newCompanyId=Company2Exists->getId();
    }
    if(Company1Exists->workers->getSize() == Company1Exists->workers->getSize()){
        if(Company1Exists->getId()<Company2Exists->getId()){
            newCompanyId=Company1Exists->getId();
        }
        if(Company1Exists->getId()>Company2Exists->getId()){
            newCompanyId=Company2Exists->getId();
        }

    }

    Worker* mergedBestWorker;
    if(Company1Exists->getBestWorker()==NULL&&Company2Exists->getBestWorker()==NULL){
        mergedBestWorker=NULL;
    }
    if(Company1Exists->getBestWorker()==NULL&&Company2Exists->getBestWorker()!=NULL){
        mergedBestWorker=Company2Exists->getBestWorker();
    }
    if(Company1Exists->getBestWorker()!=NULL&&Company2Exists->getBestWorker()==NULL){
        mergedBestWorker=Company1Exists->getBestWorker();
    }
    if(Company2Exists->getBestWorker()==NULL||Company1Exists->getBestWorker()->getRank()>Company2Exists->getBestWorker()->getRank()){
        mergedBestWorker=Company1Exists->getBestWorker();
    }else{
        mergedBestWorker=Company2Exists->getBestWorker();
    }
    if(mergedBestWorker->getRank()<minimalRank){
        mergedBestWorker=NULL;
    }
    Worker** allWorkers1;
    allWorkers1=Company1Exists->workers->createDataArray();
    Worker** allWorkers2;
    allWorkers2 =Company2Exists->workers->createDataArray();

    int j=0;
    for (int r=0;r<Company1Exists->workers->getSize();r++){
        if(allWorkers1[r]->getRank()<minimalRank){
            j++;
        }
    }
    int numFinalWorkers1 = Company1Exists->workers->getSize()-j;

    int i=0;
    for (int k=0;k<Company2Exists->workers->getSize();k++){
        if(allWorkers2[k]->getRank()<minimalRank){
            i++;
        }
    }
    int numFinalWorkers2 = Company2Exists->workers->getSize()-i;

    Worker** finalWorkers1=(Worker**) malloc(sizeof(Worker) * numFinalWorkers1);
    Worker** finalWorkers2=(Worker**) malloc(sizeof(Worker) * numFinalWorkers2);
    int t=0;
    int k=0;
    int s=0;
    for(k=j;k<Company1Exists->workers->getSize();k++){
        finalWorkers1[t]=allWorkers1[k];
        finalWorkers1[t]->setCompany(this->Companies->find(newCompanyId));
        t++;
    }
    for(k=0;k<j;k++){
        allWorkers1[k]->setCompany(NULL);
    }
    t=0;
    for(s=i;s<Company2Exists->workers->getSize();s++){
        finalWorkers2[t]=allWorkers2[s];
        finalWorkers2[t]->setCompany(this->Companies->find(newCompanyId));
        t++;
    }
    for(s=0;s<i;s++){
        allWorkers2[s]->setCompany(NULL);
    }

    int size = numFinalWorkers1+numFinalWorkers2;
    Worker** mergedWorkers=(Worker**) malloc(sizeof(Worker) * size);
    merge(finalWorkers1,numFinalWorkers1,finalWorkers2,numFinalWorkers2,mergedWorkers);
    free(finalWorkers1);
    free(finalWorkers2);
    delete[] allWorkers1;
    delete[] allWorkers2;
    delete(this->Companies->find(newCompanyId)->workers);

    WorkersTree_t* mergeWorkerTree = mergeWorkerTree->createTree(size);
    rankAndId** keyWorkers = (rankAndId**) malloc(sizeof(Worker) * size);
    for(i=0;i<size;i++){
        keyWorkers[i]=mergedWorkers[i]->getRankAndId();
    }
    mergeWorkerTree->fill(keyWorkers,mergedWorkers);
    free(keyWorkers);
    free(mergedWorkers);
    this->Companies->find(newCompanyId)->workers=mergeWorkerTree;
    this->Companies->find(newCompanyId)->setBestWorker(mergedBestWorker);
    if(newCompanyId==companyID1){
        this->Companies->remove(companyID2);
        delete(Company2Exists);
    }
    else {
        this->Companies->remove(companyID1);
        delete(Company1Exists);
    }
    return SUCCESS;
}
StatusType HighTech::changeRank(int workerID, int newRank){
    if(workerID <= 0 || newRank <= 0 ){
        return INVALID_INPUT;
    }

    Worker* WorkerExists = this->Workers->find(workerID);
    if( WorkerExists==NULL){
        return FAILURE;
    }
    int newWorkerId=WorkerExists->getId();
    int newWorkerRank = newRank;
    Company* newCompany=WorkerExists->getCompany();
    StatusType status =removeWorker(workerID);
    if(status!=SUCCESS){
        return status;
    }
    status=addWorker(newWorkerId,newWorkerRank);
    if(status!=SUCCESS){
        return status;
    }
    if(newCompany!=NULL){
        addworkerToCompany(newWorkerId,newCompany->getId());
    }
    return SUCCESS;
}
StatusType HighTech::getBestWorker(int companyID, int *workerID){
    if(workerID==NULL || companyID==0){
        return INVALID_INPUT;
    }
    if(companyID<0){
        *workerID=this->bestWorker->getId();
    }else{
        Company* CompanyExists = this->Companies->find(companyID);
        if( CompanyExists==NULL){
            return FAILURE;
        }
        if(CompanyExists->getBestWorker()==NULL){
            *workerID=-1;
            return SUCCESS;
        }
        *workerID=CompanyExists->getBestWorker()->getId();
    }
    return SUCCESS;
}
StatusType HighTech::getCompanyWorkersByRank (int companyID, int **workers, int *numOfWorkers){
    if(companyID == 0 || workers == NULL || numOfWorkers == NULL){
        return INVALID_INPUT;
    }
    if(companyID<0){
        *numOfWorkers=this->AllWorkersTree->getSize();
        *workers=(int*)malloc(sizeof(int) * (*numOfWorkers));
        if(*workers == NULL){
            return ALLOCATION_ERROR;
        }
        getTree(this->AllWorkersTree, *workers,*numOfWorkers);
        reverse(*workers,*workers+*numOfWorkers-1);
        return SUCCESS;
    }
    Company* CompanyExists = this->Companies->find(companyID);
    if(CompanyExists==NULL){
        return FAILURE;
    }
    *numOfWorkers=CompanyExists->workers->getSize();
    *workers=(int*)malloc(sizeof(int) * (*numOfWorkers));
    if(*workers == NULL){
        return ALLOCATION_ERROR;
    }
    getTree(CompanyExists->workers, *workers,*numOfWorkers);
    reverse(*workers,*workers+*numOfWorkers-1);
    return SUCCESS;
}


void HighTech::Quit(){
    AllWorkersTree->dataDelete();
    Companies->dataDelete();
    delete(this->Companies);
    delete(this->Workers);
    delete(this->AllWorkersTree);

}

void HighTech::getTree(AllWorkersTreeRank_t* AllWorkersTree, int *workers,int size){
    for (AllWorkersTreeIterator it = AllWorkersTree->begin(); it != AllWorkersTree->end(); ++it) {
        *workers=(*it)->getId();
        workers++;
    }
}
void HighTech::reverse(int* start, int* end){
    int* first= start;
    int* last= end;
    int temp=0;
    while (first!=last &&!(first>last)) {
        temp = *first;
        *first = *last;
        *last = temp;
        first++;
        last--;
    }
    return;
}

void HighTech::merge(Worker* a[], int m, Worker* b[], int n, Worker* sorted[]) {
    int i, j, k;
    j = k = 0;
    for (i = 0; i < m + n;) {
        if (j < m && k < n) {
            if (a[j]->getRank() < b[k]->getRank()) {
                sorted[i] = a[j];
                j++;
            } else {
                sorted[i] = b[k];
                k++;
            }
            i++;
        } else if (j == m) {
            for (; i < m + n;) {
                sorted[i] = b[k];
                k++;
                i++;
            }
        } else {
            for (; i < m + n;) {
                sorted[i] = a[j];
                j++;
                i++;
            }
        }
    }
}
