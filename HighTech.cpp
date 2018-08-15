#include "HighTech.h"
#include <stdio.h>
#include <stdlib.h>
    
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
    if(companyID1 <= 0 || companyID2 <= 0 || companyID1 ==companyID2 || minimalRank<=0){
        return INVALID_INPUT;
    }

    Company* Company1Exists = this->Companies->Find(companyID1);
    Company* Company2Exists = this->Companies->Find(companyID2);
    if( Company1Exists==NULL || Company2Exists==NULL){
        return FAILURE;
    }

    Worker** workersCompany1;
    Worker** workersCompany2;
    int workersCompanyLength1;
    int workersCompanyLength2;


    Company1Exists->workers->treeArrayByData(&workersCompanyLength1,&workersCompany1);
    Company2Exists->workers->treeArrayByData(&workersCompanyLength2,&workersCompany2);

    int goodWorkers1=0;//number of workers in company 1 with higher than minimal rank
    int goodWorkers2=0;//number of workers in company 2 with higher than minimal rank

    int i=0,j=0;//indexes for while loops
    while((workersCompany1)[i]->getRank()>=minimalRank){
    goodWorkers1++;
    i++;
    }

    while((workersCompany2)[j]->getRank()>=minimalRank){
        goodWorkers2++;
        j++;
    }

    Worker** qualifiedWorkers1=(Worker**) malloc(sizeof(Worker) * (goodWorkers1));
    Worker** qualifiedWorkers2=(Worker**) malloc(sizeof(Worker) * (goodWorkers2));



    //substracting the minimal workers from origin array

    for(int k=0;k<goodWorkers1;k++){
        qualifiedWorkers1[k]=workersCompany1[k];
    }

    for(int t=0;t<goodWorkers2;t++){
        qualifiedWorkers2[t]=workersCompany2[t];
    }


    Worker** workerCompany3 = (Worker**) malloc(sizeof(Worker) * (goodWorkers1+goodWorkers2));

    //merge qualifiedWorkers1+qualifiedWorkers2 into workerCompany3;

    mergeArrays(qualifiedWorkers1,goodWorkers1,qualifiedWorkers2,goodWorkers2,workerCompany3);

    //calc the depth of full tree with good workers 1 +2
    int depth= (int)log2(goodWorkers1+goodWorkers2);

    //creating a complete tree to push the final merged good workers
    AvlTree<rankAndId* , Worker*, rankThanId>* mergedCompanyWorkers=AvlTree::FullTree(depth+1);

    int numDelete=exp(2,1+depth)-1-(goodWorkers1+goodWorkers2);

    AvlTree::deletepOrder(mergedCompanyWorkers,&numDelete);
    int index=0;
    int len=goodWorkers1+goodWorkers2;

    AvlTree::reverseInOrderCopyArray(mergedCompanyWorkers,workerCompany3,&len,&index);

    //set new merged compamy settings
    int mergedCompanyID;

    if(workersCompanyLength1>workersCompanyLength2){
        mergedCompanyID=companyID1;
    }else if(workersCompanyLength1==workersCompanyLength2){
        if(companyID1<companyID2){
            mergedCompanyID=companyID1;
        }else{
            mergedCompanyID=companyID2;
        }
    }else{
        mergedCompanyID=companyID2;
    }









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
    while (*AllWorkersIterator != NULL){
        printf("'worker id - %d\n",(*AllWorkersIterator)->getId());
        AllWorkersIterator++;
    }
}

