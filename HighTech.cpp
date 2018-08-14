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
    WorkerExists->getCompany()->workers->Delete(WorkerExists->getCompany()->workers->Find(workerID)->getId());
    this->AllWorkersTree->Delete(workerID);
    return SUCCESS;
}
StatusType HighTech::mergeCompanies(int companyID1, int companyID2, int minimalRank){
    return SUCCESS;
}
StatusType HighTech::changeRank(int workerID, int newRank){
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
        deletWorkerTree(companiesIterator.value()->workers);
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