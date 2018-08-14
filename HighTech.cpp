#include "HighTech.h"
#include <stdio.h>
#include <stdlib.h>
    
HighTech::HighTech(){
    this->Companies=new CompaniesTree_t();
    //this->Workers=new WorkersTree_t();
    this->AllWorkersTree=new AllWorkersTree_t();
    this->bestWorker=NULL;
}
StatusType HighTech::addWorker(int workerID, int rank){
    if(workerID<=0 || rank<=0){
        return INVALID_INPUT;
    }
    if(this->AllWorkersTree->Exists(workerID) == true){
        return FAILURE;
    }
    Worker* newWorker;
    try{
        newWorker=new Worker(workerID,rank);
    } catch (std::bad_alloc&) {
		return ALLOCATION_ERROR;
	}
    if(this->AllWorkersTree->Insert(workerID,newWorker)==false){
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
    Worker* WorkerExists = this->AllWorkersTree->Find(workerID);
    Company* CompanyExists = this->Companies->Find(companyID);
    if( WorkerExists==NULL || CompanyExists==NULL){
        return FAILURE;
    }
    if(CompanyExists->workers->Insert(workerID,WorkerExists)==false){
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
    Worker* WorkerExists = this->AllWorkersTree->Find(workerID);
    if( WorkerExists==NULL){
        return FAILURE;
    }
    WorkerExists->getCompany()->workers->Delete(WorkerExists->getCompany()->workers->Find(workerID));
    this->AllWorkersTree->Delete(workerID);
    return SUCCESS;
}
StatusType HighTech::mergeCompanies(int companyID1, int companyID2, int minimalRank){
    if(companyID1 <= 0 || companyID2 <= 0 || minimalRank<=0){
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

    int minimalWorkers1=0;//number of workers in company 1 with lower than minimal rank
    int minimalWorkers2=0;//number of workers in company 2 with lower than minimal rank

    int i=0,j=0;//indexes for while loops
    while((workersCompany1)[i]->getRank()<minimalRank){
    minimalWorkers1++;
    i++;
    }

    while((workersCompany2)[j]->getRank()<minimalRank){
        minimalWorkers1++;
        j++;
    }





return SUCCESS;
}
StatusType HighTech::changeRank(int workerID, int newRank){
    if(workerID <= 0 || newRank <= 0 ){
        return INVALID_INPUT;
    }

    Worker* WorkerExists = this->AllWorkersTree->Find(workerID);
    if( WorkerExists==NULL){
        return FAILURE;
    }

    Company* workerCompany= WorkerExists->getCompany();

    //worker doesn't belong to a company
    if(workerCompany==NULL){
    this->removeWorker(workerID);
    this->addWorker(workerID,newRank);
    }
    //worker belong to a company
    else{
        int workerCompanyId= WorkerExists->getCompany()->getId();
        this->removeWorker(workerID);
        this->addWorker(workerID,newRank);
        this->addworkerToCompany(workerID,workerCompanyId);
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
    return SUCCESS;
}

void HighTech::Quit(){
    delete(this->bestWorker);
    deleteWorkerTree(this->AllWorkersTree);
    delete(this->AllWorkersTree);
    deleteCompanyTree(this->Companies);
    delete(this->Companies);
}

void HighTech::getTree(AllWorkersTree_t* AllWorkersTree, int *workers){
    AllWorkersTreeIterator AllWorkersIterator = AllWorkersTree->GetIterator();
    while (*AllWorkersIterator != NULL){
        *workers = (*AllWorkersIterator)->getId();
        workers++;
        AllWorkersIterator++;
    }
}

void HighTech::deleteWorkerTree(AllWorkersTree_t* AllWorkersTree){
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
void HighTech::printTree(AllWorkersTree_t* PostTree){
    AllWorkersTreeIterator AllWorkersIterator = AllWorkersTree->GetIterator();
    while (*AllWorkersIterator != NULL){
        printf("'worker id - %d\n",(*AllWorkersIterator)->getId());
        AllWorkersIterator++;
    }
}