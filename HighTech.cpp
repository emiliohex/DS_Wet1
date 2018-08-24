
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
    //if(WorkerExists==NULL){printf("WORKER\n");}
    //if(CompanyExists==NULL){printf("COMPANY\n");}
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
    //printf("node company - %d\n",CompanyExists->workers->getSize());
    printTree(CompanyExists->workers);
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
    //printf("OK1 \n");
    rankAndId* newRankAndId = WorkerExists->getRankAndId();
    Company* newCompany = WorkerExists->getCompany();
    WorkerExists->setCompany(NULL);
    //printf("OK2 \n");
    //printTree(newCompany->workers);
    if(newCompany!=NULL){
        newCompany->workers->remove(newRankAndId);
        if(newCompany->getBestWorker()== WorkerExists){
            newCompany->setBestWorker(newCompany->workers->getMax());
        }
        //printf("node company - %d\n",newCompany->workers->getSize());
        //newCompany->workers->remove(newRankAndId);
        //printTree(newCompany->workers);
    }
    //printf("OK3 \n");
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
    //printf("OK1 - %d\n",Company1Exists->getId());
    int newCompanyId;
    if(Company1Exists->workers->getSize() > Company1Exists->workers->getSize()){
        newCompanyId=Company1Exists->getId();
    }
    if(Company1Exists->workers->getSize() < Company1Exists->workers->getSize()){
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

    //printf("OK2 - %d\n",newCompanyId);
    //printf("OK3 - %d\n",Company1Exists->getBestWorker()->getRank());
    //printf("OK3 - %d\n",Company2Exists->getBestWorker()->getRank());
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
        //printf("OK41\n");
        mergedBestWorker=Company1Exists->getBestWorker();
    }else{
        //printf("OK42\n");
        mergedBestWorker=Company2Exists->getBestWorker();
    }
    if(mergedBestWorker->getRank()<minimalRank){
        //printf("OK43\n");
        mergedBestWorker=NULL;
    }
    //printf("Company1Exists->workers->getSize()-%d\n",Company1Exists->workers->getSize());
    Worker** allWorkers1;
    allWorkers1=Company1Exists->workers->createDataArray();
    //printf("OK4\n");
    Worker** allWorkers2;
    allWorkers2 =Company2Exists->workers->createDataArray();
    //printf("OK5\n");

    int j=0;
    //printf("Company1Exists->workers->getSize() - %d\n",Company1Exists->workers->getSize());
    for (int r=0;r<Company1Exists->workers->getSize();r++){
        //printf("allWorkers1[%d]->getRank() - %d\n",r,allWorkers1[r]->getRank());
        if(allWorkers1[r]->getRank()<minimalRank){
            j++;
        }
    }
    /*while(j<Company1Exists->workers->getSize()&& allWorkers1[j]->getRank()>=minimalRank){
        printf("rank - %d , j - %d\n",allWorkers1[j]->getRank(),j);
        j++;
    }
    if(j>Company1Exists->workers->getSize()) j--;*/
    //printf("j - %d\n",j);
    int numFinalWorkers1 = Company1Exists->workers->getSize()-j;

    //printf("Company1Exists->workers->getSize() - %d\n",Company1Exists->workers->getSize());
    int i=0;
    for (int k=0;k<Company2Exists->workers->getSize();k++){
        if(allWorkers2[k]->getRank()<minimalRank){
            i++;
        }
    }
    /*while(i<Company2Exists->workers->getSize() && allWorkers2[i]->getRank()>=minimalRank){
        i++;
    }
    if(i>Company2Exists->workers->getSize()) i--;*/
    //printf("i - %d\n",i);
    int numFinalWorkers2 = Company2Exists->workers->getSize()-i;
    //printf("numFinalWorkers1 - %d\n",numFinalWorkers1);
    //printf("numFinalWorkers2 - %d\n",numFinalWorkers2);
    //printf("OK5\n");

    Worker** finalWorkers1=(Worker**) malloc(sizeof(Worker) * numFinalWorkers1);
    Worker** finalWorkers2=(Worker**) malloc(sizeof(Worker) * numFinalWorkers2);
    int t=0;
    int k=0;
    int s=0;
    //printf("Company1Exists->workers->getSize() - %d\n",Company1Exists->workers->getSize());
    for(k=j;k<Company1Exists->workers->getSize();k++){
        finalWorkers1[t]=allWorkers1[k];
        finalWorkers1[t]->setCompany(this->Companies->find(newCompanyId));
        //printf("allWorkers1[t1] - %d\n",allWorkers1[k]->getId());
        t++;
    }
    for(k=0;k<j;k++){
        allWorkers1[k]->setCompany(NULL);
        //printf("remove[t2] - %d\n",allWorkers1[k]->getId());
    }
    t=0;
    for(s=i;s<Company2Exists->workers->getSize();s++){
        finalWorkers2[t]=allWorkers2[s];
        finalWorkers2[t]->setCompany(this->Companies->find(newCompanyId));
        //printf("allWorkers2[t1] - %d\n",allWorkers2[s]->getId());
        t++;
    }
    for(s=0;s<i;s++){
        allWorkers2[s]->setCompany(NULL);
    }


   // printf("OK6\n");
    //printf("numFinalWorkers1 - %d\n",numFinalWorkers1);
    //printf("numFinalWorkers2 - %d\n",numFinalWorkers2);
    int size = numFinalWorkers1+numFinalWorkers2;
    Worker** mergedWorkers=(Worker**) malloc(sizeof(Worker) * size);
    merge(finalWorkers1,numFinalWorkers1,finalWorkers2,numFinalWorkers2,mergedWorkers);
    free(finalWorkers1);
    free(finalWorkers2);
    delete[] allWorkers1;
    delete[] allWorkers2;
    //reverse(mergedWorkers,mergedWorkers+size-1);
    //printf("OK7\n");
    //this->Companies->find(newCompanyId)->workers->deleteTree();
    delete(this->Companies->find(newCompanyId)->workers);
    /*  Company* mergedCompany;
      try{
          mergedCompany=new Company(newCompanyId);
      } catch (std::bad_alloc&) {
          return ALLOCATION_ERROR;
      }*/
    //printf("OK8\n");

    WorkersTree_t* mergeWorkerTree = mergeWorkerTree->createTree(size);

    //printTree(mergeWorkerTree);
    //int depth = (int)log2(size);
    //int deleteNum=(int)(pow(2,1+depth)-1-size);
    //printf("depth is - %d\n",depth);
    //printf("size is - %d\n",size);
    //printf("deleteNum is - %d\n",deleteNum);
    //printf("OK9\n");
    //mergeWorkerTree->deleteFullTreeOrder(deleteNum);
    //printf("OK10 - %d\n",size);
    rankAndId** keyWorkers = (rankAndId**) malloc(sizeof(Worker) * size);
    for(i=0;i<size;i++){
        keyWorkers[i]=mergedWorkers[i]->getRankAndId();
    }
    mergeWorkerTree->fill(keyWorkers,mergedWorkers);
    free(keyWorkers);
    free(mergedWorkers);
    //printTree(mergeWorkerTree);
    //printf("OK11\n");
    this->Companies->find(newCompanyId)->workers=mergeWorkerTree;
    this->Companies->find(newCompanyId)->setBestWorker(mergedBestWorker);
    //printf("nodes - %d\n",mergedCompany->workers->FindMax()->getId());
    //printTree(mergedCompany->workers);
    if(newCompanyId==companyID1){
        this->Companies->remove(companyID2);
        delete(Company2Exists);
    }
    else {
        this->Companies->remove(companyID1);
        delete(Company1Exists);
    }
    //this->Companies->add(mergedCompany->getId(),mergedCompany);
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
    //Worker* newWorkerExists = this->Workers->find(newWorkerId);
    if(newCompany!=NULL){
        addworkerToCompany(newWorkerId,newCompany->getId());
    }
    //newWorkerExists->setCompany(newCompany);
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
        //delete *workers;
        *workers=(int*)malloc(sizeof(int) * (*numOfWorkers));
        //int* allWorkers = (int*)malloc(sizeof(allWorkers) * (*numOfWorkers));
        if(*workers == NULL){
            return ALLOCATION_ERROR;
        }
        getTree(this->AllWorkersTree, *workers,*numOfWorkers);
        //printTree(this->AllWorkersTree);
        reverse(*workers,*workers+*numOfWorkers-1);
        //*workers = allWorkers;
        return SUCCESS;
    }
    Company* CompanyExists = this->Companies->find(companyID);
    if(CompanyExists==NULL){
        return FAILURE;
    }
    *numOfWorkers=CompanyExists->workers->getSize();
    //printf("numOfWorkers-%d\n",*numOfWorkers);
    //delete *workers;
    *workers=(int*)malloc(sizeof(int) * (*numOfWorkers));
    //int* companyWorkers = (int*)malloc(sizeof(companyWorkers) * (*numOfWorkers));
    if(*workers == NULL){
        return ALLOCATION_ERROR;
    }
    getTree(CompanyExists->workers, *workers,*numOfWorkers);
    printTree(CompanyExists->workers);
    reverse(*workers,*workers+*numOfWorkers-1);
    //*workers = companyWorkers;
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
void HighTech::deleteWorkerTree(AllWorkersTreeRank_t* AllWorkersTree){
    if(AllWorkersTree==NULL){
        return;
    }
    for (AllWorkersTreeIterator it = AllWorkersTree->begin(); it != AllWorkersTree->end(); ++it) {
        //printf("worker id - %d\n",(*AllWorkersIterator)->getId());
        delete(*it);
    }

}
void HighTech::deleteCompanyTree(CompaniesTree_t* Companies){
    if(Companies==NULL){
        return;
    }
    for (CompaniesTreeIterator it = Companies->begin(); it != Companies->end(); ++it) {
        //printf("worker id - %d\n",(*AllWorkersIterator)->getId());
        deleteWorkerTree((*it)->workers);
        delete(*it);
    }
}
void HighTech::printTree(AllWorkersTreeRank_t* workerTree){
    for (AllWorkersTreeIterator it = workerTree->begin(); it != workerTree->end(); ++it) {
        //printf("worker id - %d , worker rank - %d\n",(*it)->getId(),(*it)->getRank());
    }
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
