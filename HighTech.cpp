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
    if(this->Workers->Exists(workerID) == true){
        return FAILURE;
    }
    Worker* newWorker;
    try{
        newWorker=new Worker(workerID,rank);
    } catch (std::bad_alloc&) {
		return ALLOCATION_ERROR;
	}
    if(this->Workers->Insert(workerID, newWorker) == false){
        return ALLOCATION_ERROR;
    }
    rankAndId* newRankAndId = newWorker->getRankAndId();
    if(this->AllWorkersTree->Insert(newRankAndId, newWorker) == false){
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
    if(this->Companies->Exists(companyID) == true){
        return FAILURE;
    }
    Company* newCompany;
    try{
        newCompany=new Company(companyID);
    } catch (std::bad_alloc&) {
		return ALLOCATION_ERROR;
	}
    if(this->Companies->Insert(companyID,newCompany)==false){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;

}
StatusType HighTech::addworkerToCompany(int workerID, int companyID){
    if(workerID<=0 || companyID<=0){
        return INVALID_INPUT;
    }
    Worker* WorkerExists = this->Workers->Find(workerID);
    Company* CompanyExists = this->Companies->Find(companyID);
    if( WorkerExists==NULL || CompanyExists==NULL){
        return FAILURE;
    }
    rankAndId* newRankAndId = WorkerExists->getRankAndId();
    if(CompanyExists->workers->Insert(newRankAndId, WorkerExists)==false){
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
    Worker* WorkerExists = this->Workers->Find(workerID);
    if( WorkerExists==NULL){
        return FAILURE;
    }
    rankAndId* newRankAndId = WorkerExists->getRankAndId();
    Company* newCompany = WorkerExists->getCompany();
    if(newCompany!=NULL){
        newCompany->workers->Delete(newRankAndId);
    }
    this->Workers->Delete(workerID);
    this->AllWorkersTree->Delete(newRankAndId);
    return SUCCESS;
}

StatusType HighTech::mergeCompanies(int companyID1, int companyID2, int minimalRank){
    if(companyID1 <= 0 || companyID2 <= 0 || companyID1 == companyID2 || minimalRank<=0){
        return INVALID_INPUT;
    }

    Company* Company1Exists = this->Companies->Find(companyID1);
    Company* Company2Exists = this->Companies->Find(companyID2);
    if( Company1Exists==NULL || Company2Exists==NULL){
        return FAILURE;
    }
    //printf("OK1 - %d\n",Company1Exists->getId());
    int newCompanyId;
    if(Company1Exists->workers->getNodesCount() >= Company1Exists->workers->getNodesCount()){
        newCompanyId=Company1Exists->getId();
    }else{
        newCompanyId=Company2Exists->getId();
    }
    //printf("OK2 - %d\n",newCompanyId);
    //printf("OK3 - %d\n",Company1Exists->getBestWorker()->getRank());
    Worker* mergedBestWorker;
    if(Company1Exists->getBestWorker()==NULL||Company1Exists->getBestWorker()->getRank()>=Company2Exists->getBestWorker()->getRank()){
        mergedBestWorker=Company1Exists->getBestWorker();
    }else{
        mergedBestWorker=Company2Exists->getBestWorker();
    }
    if(mergedBestWorker->getRank()<minimalRank){
        mergedBestWorker=NULL;
    }
    //printf("mergedBestWorker-%d\n",mergedBestWorker->getId());
    Worker** allWorkers1;
    allWorkers1=Company1Exists->workers->createDataArray();
    Worker** allWorkers2;
    allWorkers2 =Company2Exists->workers->createDataArray();
    //printf("OK4\n");

    int j=0;
    while(j<Company1Exists->workers->getNodesCount()&& allWorkers1[j]->getRank()>=minimalRank){
        //printf("rank - %d , j - %d\n",allWorkers1[j]->getRank(),j);
        j++;
    }
    if(j>Company1Exists->workers->getNodesCount()) j--;
    //printf("j - %d\n",j);
    int numFinalWorkers1 = j;

    int i=0;
    while(i<Company2Exists->workers->getNodesCount() && allWorkers2[i]->getRank()>=minimalRank){
        i++;
    }
    if(i>Company2Exists->workers->getNodesCount()) i--;
    //printf("i - %d\n",i);
    int numFinalWorkers2 = i;
    //printf("numFinalWorkers1 - %d\n",numFinalWorkers1);
    //printf("numFinalWorkers2 - %d\n",numFinalWorkers2);
    //printf("OK5\n");

    Worker** finalWorkers1=(Worker**) malloc(sizeof(Worker) * numFinalWorkers1);
    Worker** finalWorkers2=(Worker**) malloc(sizeof(Worker) * numFinalWorkers2);
    int t=0;
    int k=0;
    int s=0;
    //printf("Company2Exists->workers->getNodesCount() - %d\n",Company2Exists->workers->getNodesCount());
    if(j==Company1Exists->workers->getNodesCount()){
        for(k=0;k<Company1Exists->workers->getNodesCount();k++){
            finalWorkers1[t]=allWorkers1[k];
            //printf("allWorkers1[t1] - %d\n",allWorkers1[k]->getId());
            t++;
        }
    }else{
        for(k=0;k<=j;k++){
            //printf("allWorkers1[t2] - %d\n",allWorkers1[k]->getId());
            finalWorkers1[t]=allWorkers1[k];
            t++;
        }
    }
    t=0;
    if(i==Company2Exists->workers->getNodesCount()){
        for(s=0;s<Company2Exists->workers->getNodesCount();s++){
            finalWorkers2[t]=allWorkers2[s];
            //printf("allWorkers2[t1] - %d\n",allWorkers2[s]->getId());
            t++;
        }
    }else{
        for(s=0;s<=s;s++){
            //printf("allWorkers2[t2] - %d\n",allWorkers2[i]->getId());
            finalWorkers2[t]=allWorkers2[s];
            t++;
        }
    }

    //printf("OK6\n");
    //printf("numFinalWorkers1 - %d\n",numFinalWorkers1);
    //printf("numFinalWorkers2 - %d\n",numFinalWorkers2);
    int size = numFinalWorkers1+numFinalWorkers2;
    Worker** mergedWorkers=(Worker**) malloc(sizeof(Worker) * size);
    merge(finalWorkers1,numFinalWorkers1,finalWorkers2,numFinalWorkers2,mergedWorkers);
    for(i=0;i<numFinalWorkers2+numFinalWorkers1;i++){
        //printf("mergedWorkers[t2] - %d\n",mergedWorkers[i]->getId());
    }
    //printf("OK7\n");

    Company* mergedCompany;
    try{
        mergedCompany=new Company(newCompanyId);
    } catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
    //printf("OK8\n");

    WorkersTree_t* mergeWorkerTree = new WorkersTree_t(size);
    int depth = (int)log2(size);
    int deleteNum=(int)(pow(2,1+depth)-1-size);
    //printf("depth is - %d\n",depth);
    //printf("size is - %d\n",size);
    //printf("deleteNum is - %d\n",deleteNum);
    //printf("OK9\n");
    mergeWorkerTree->deleteFullTreeOrder(deleteNum);
    //printf("OK10 - %d\n",size);
    mergeWorkerTree->InOrderCopyArray(mergedWorkers,size,0);
    //printf("OK11\n");
    mergedCompany->workers=mergeWorkerTree ;
    mergedCompany->setBestWorker(mergedBestWorker);
    //printf("nodes - %d\n",mergedCompany->workers->FindMax()->getId());
    printTree(mergedCompany->workers);
    this->Companies->Delete(companyID2);
    this->Companies->Delete(companyID1);
    this->Companies->Insert(mergedCompany->getId(),mergedCompany);
return SUCCESS;
}
StatusType HighTech::changeRank(int workerID, int newRank){
    if(workerID <= 0 || newRank <= 0 ){
        return INVALID_INPUT;
    }

    Worker* WorkerExists = this->Workers->Find(workerID);
    if( WorkerExists==NULL){
        return FAILURE;
    }
    rankAndId* newRankAndId = WorkerExists->getRankAndId();
    Company* workerCompany= WorkerExists->getCompany();
    if(workerCompany!=NULL){
        workerCompany->workers->Delete(newRankAndId);
    }
    this->AllWorkersTree->Delete(newRankAndId);
    this->Workers->Delete(workerID);

    WorkerExists->changeRank(newRank);

    newRankAndId = WorkerExists->getRankAndId();
    if(workerCompany!=NULL){
        if(workerCompany->workers->Insert(newRankAndId,WorkerExists)==false){
            return ALLOCATION_ERROR;
        }
    }
    if(this->AllWorkersTree->Insert(newRankAndId,WorkerExists)==false){
        return ALLOCATION_ERROR;
    }
    if(this->Workers->Insert(workerID,WorkerExists)==false){
        return ALLOCATION_ERROR;
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
        Company* CompanyExists = this->Companies->Find(companyID);
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
        *numOfWorkers=this->AllWorkersTree->getNodesCount();
        int* allWorkers = (int*)malloc(sizeof(allWorkers) * (*numOfWorkers));
        if(allWorkers == NULL){
            return ALLOCATION_ERROR;
        }
        getTree(this->AllWorkersTree, allWorkers);
        printTree(this->AllWorkersTree);
        *workers = allWorkers;
        return SUCCESS;
    }
    Company* CompanyExists = this->Companies->Find(companyID);
    if(CompanyExists==NULL){
        return FAILURE;
    }
    *numOfWorkers=CompanyExists->workers->getNodesCount();
    int* companyWorkers = (int*)malloc(sizeof(companyWorkers) * (*numOfWorkers));
    if(companyWorkers == NULL){
        return ALLOCATION_ERROR;
    }
    getTree(CompanyExists->workers, companyWorkers);
    printTree(CompanyExists->workers);
    *workers = companyWorkers;
    return SUCCESS;
}

void HighTech::Quit(){
    delete(this->bestWorker);
    deleteWorkerTree(this->AllWorkersTree);
    delete(this->AllWorkersTree);
    deleteCompanyTree(this->Companies);
    delete(this->Companies);
}

void HighTech::getTree(AllWorkersTreeRank_t* AllWorkersTree, int *workers){
    AllWorkersTreeIterator AllWorkersIterator = AllWorkersTree->GetIterator();
    while (*AllWorkersIterator != NULL){
        *workers = (*AllWorkersIterator)->getId();
        workers++;
        AllWorkersIterator++;
    }
}

void HighTech::deleteWorkerTree(AllWorkersTreeRank_t* AllWorkersTree){
    if(AllWorkersTree==NULL){
        return;
    }
    AllWorkersTreeIterator AllWorkersIterator = AllWorkersTree->GetIterator();
    while(*AllWorkersIterator!=NULL){
        //printf("worker id - %d\n",(*AllWorkersIterator)->getId());
        delete(*AllWorkersIterator);
        AllWorkersIterator++;
    }

}
void HighTech::deleteCompanyTree(CompaniesTree_t* Companies){
    if(Companies==NULL){
        return;
    }
    CompaniesTreeIterator companiesIterator = Companies->GetIterator();
    while(*companiesIterator!=NULL){
       deleteWorkerTree(companiesIterator.value()->workers);
        delete(*companiesIterator);
        companiesIterator++;
    }
}
void HighTech::printTree(AllWorkersTreeRank_t* workerTree){
    AllWorkersTreeIterator AllWorkersIterator = workerTree->GetIterator();
    //printf("OK15\n");
    while (*AllWorkersIterator != NULL){
        //printf("worker id - %d\n",(*AllWorkersIterator)->getId());
        AllWorkersIterator++;
    }
}

void HighTech::merge(Worker* a[], int m, Worker* b[], int n, Worker* sorted[]) {
    int i, j, k;

    j = k = 0;

    for (i = 0; i < m + n;) {
        if (j < m && k < n) {
            if (a[j]->getRank() > b[k]->getRank()) {
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

