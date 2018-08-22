#include <iostream>
#include "library1.h"
#include <cassert>
#include <stdlib.h>



void testAddCompany1(void* DS){
    assert(addCompany(DS,1)==SUCCESS);
    assert(addCompany(DS,2)==SUCCESS);
    assert(addCompany(DS,3)==SUCCESS);
    assert(addCompany(DS,4)==SUCCESS);
    assert(addCompany(DS,5)==SUCCESS);
    assert(addCompany(NULL,1)==INVALID_INPUT);
    assert(addCompany(DS,-1)==INVALID_INPUT);
    assert(addCompany(DS,0)==INVALID_INPUT);
}
void testAddCompany2(void* DS){
    for(int i=1;i<6;i++){
        assert(addCompany(DS,i)==FAILURE);
    }
    for(int i=100;i<110;i++){
        assert(addCompany(DS,i)==SUCCESS);
    }
    std::cout<<"Passed add company test..."<<std::endl;
}
void testAddWorker(void* DS){
    assert(addWorker(NULL, 1, 1)==INVALID_INPUT);
    assert(addWorker(DS, -1, 1)==INVALID_INPUT);
    assert(addWorker(DS, 1, -1)==INVALID_INPUT);
    assert(addWorker(DS, 1, 0)==INVALID_INPUT);
    assert(addWorker(DS, 0, 1)==INVALID_INPUT);
    assert(addWorker(DS, 1, 1)==SUCCESS);
    for (int i = 0; i < 10; ++i) {
        assert(addWorker(DS,60+i,55+i)==SUCCESS);
    }
    for (int i = 0; i < 10; ++i) {
        assert(addWorker(DS,100+i,100+i)==SUCCESS);
    }
    for (int i = 0; i < 5; ++i) {
        assert(addWorker(DS,20+i,20+i)==SUCCESS);
    }
    //check if the workers are in the system
    for (int i = 0; i < 10; ++i) {
        assert(addWorker(DS,60+i,55+i)==FAILURE);
        assert(addWorker(DS,100+i,100+i)==FAILURE);
        if(i<5)
            assert(addWorker(DS,20+i,20+i)==FAILURE);
    }
    std::cout<<"Passed add worker test..."<<std::endl;

}
void testAddWorkerToCompany(void *DS){

    assert(addworkerToCompany(NULL, 1, 1)==INVALID_INPUT);
    assert(addworkerToCompany(DS, -1, 1)==INVALID_INPUT);
    assert(addworkerToCompany(DS, 1, -1)==INVALID_INPUT);
    assert(addworkerToCompany(DS, 1, 0)==INVALID_INPUT);
    assert(addworkerToCompany(DS, 0, 1)==INVALID_INPUT);
    assert(addworkerToCompany(DS, 1, 6)==FAILURE);
    //printf("OK1\n");
    // company 6 dose not exist
    assert(addworkerToCompany(DS, 6, 1)==FAILURE);
    // worker 6 dose not exist
   // assert(addworkerToCompany(DS,60,1)==SUCCESS);
    for (int i = 0; i < 10; ++i) {
        assert(addworkerToCompany(DS,60+i,1)==SUCCESS);
        //  Worker(i) with id 60+i rank 55+i company 1
    }
    //printf("OK2\n");
    for (int i = 0; i < 10; ++i) {
        assert(addworkerToCompany(DS,100+i,2)==SUCCESS);
        //  worker(i) with id 100+i rank 100+i company 2
    }
    //printf("OK3\n");
    for (int i = 0; i < 5; ++i) {
        assert(addworkerToCompany(DS,20+i,3)==SUCCESS);
        //  worker(i) with id 20+i score 20+i company 3
    }
   // printf("OK4\n");
    for (int i =100; i < 110; ++i) {
        assert(addworkerToCompany(DS,i,i)==SUCCESS);
        //the compnies 100-109 get one worker each that have id as the company,worker(i) changed
        //company from 2 to i
        assert(addworkerToCompany(DS,i,i)==SUCCESS);
        //worker(i) already belong to company i
    }
    std::cout<<"Passed add worker to company test..."<<std::endl;
}
void testRemoveWorker(void *DS){
    assert(removeWorker(NULL, 1)==INVALID_INPUT);
    assert(removeWorker(DS, 0)==INVALID_INPUT);
    assert(removeWorker(DS, -4)==INVALID_INPUT);
    assert(removeWorker(DS, 6)==FAILURE);
    for (int i = 0; i < 5; ++i) {
        assert(removeWorker(DS,60+i)==SUCCESS);
    }
    std::cout<<"Passed remove worker test..."<<std::endl;
}
void testChangeRank(void *DS){
    assert(changeRank(NULL,1,5)==INVALID_INPUT);
    assert(changeRank(DS,1,0)==INVALID_INPUT);
    assert(changeRank(DS,1,-1)==INVALID_INPUT);
    assert(changeRank(DS,-1,5)==INVALID_INPUT);
    assert(changeRank(DS,-1,5)==INVALID_INPUT);
    assert(changeRank(DS,0,5)==INVALID_INPUT);
    assert(changeRank(DS,60,5)==FAILURE);
    assert(changeRank(DS,1,5)==SUCCESS);
    assert(changeRank(DS,1,200)==SUCCESS);
    assert(changeRank(DS,20,24)==SUCCESS);
    std::cout<<"Passed change rank test..."<<std::endl;
}
void testMergeCompanies(void *DS){
    assert(mergeCompanies(NULL,1,2,5)==INVALID_INPUT);
    assert(mergeCompanies(DS,0,2,5)==INVALID_INPUT);
    assert(mergeCompanies(DS,-1,2,5)==INVALID_INPUT);
    assert(mergeCompanies(DS,2,0,5)==INVALID_INPUT);
    assert(mergeCompanies(DS,0,-1,5)==INVALID_INPUT);
    assert(mergeCompanies(DS,1,2,0)==INVALID_INPUT);
    assert(mergeCompanies(DS,1,2,-5)==INVALID_INPUT);
    assert(mergeCompanies(DS,2,2,5)==INVALID_INPUT);
    assert(mergeCompanies(DS,10,2,5)==FAILURE);
    assert(mergeCompanies(DS,2,10,5)==FAILURE);
    //company 10 dose not exist
    assert(mergeCompanies(DS,1,2,10)==SUCCESS);
    //will merge into company 1
    assert(mergeCompanies(DS,1,3,15)==SUCCESS);
    //will merge into company 1
    assert(addCompany(DS,2)==SUCCESS);
    assert(addCompany(DS,3)==SUCCESS);
    for (int i =100; i < 110; ++i) {
        assert(addworkerToCompany(DS,i,2)==SUCCESS);
    }
    assert(mergeCompanies(DS,1,2,10)==SUCCESS);
    //will merge into company 1 again
    assert(addCompany(DS,2)==SUCCESS);
    std::cout<<"Passed merge companies test..."<<std::endl;
}
void testGetBestWorker(void *DS){
    int best=-1;
    assert(getBestWorker(NULL,2,&best)==INVALID_INPUT);
    assert(getBestWorker(DS,0,&best)==INVALID_INPUT);
    assert(getBestWorker(DS,-2,&best)==SUCCESS);
    assert(getBestWorker(DS,0,NULL)==INVALID_INPUT);
    //printf("OK1\n");
    assert(best==1);
    assert(getBestWorker(DS,1,&best)==SUCCESS);
    //printf("OK2\n");
    assert(best==109);
    assert(getBestWorker(DS,5,&best)==SUCCESS);
    //printf("OK3\n");
    assert(best==-1);
    assert(getBestWorker(DS,6,&best)==FAILURE);
    //printf("OK4\n");
    assert(best==-1);
    std::cout<<"Passed best worker test..."<<std::endl;
}
void testGetCompanyWorkersByRank(void* DS){
    int numOfWorkers=-1;
    int *workers=&numOfWorkers;
    assert(getCompanyWorkersByRank(NULL,2,&workers,&numOfWorkers)==INVALID_INPUT);
    assert(getCompanyWorkersByRank(DS,2,NULL,&numOfWorkers)==INVALID_INPUT);
    assert(getCompanyWorkersByRank(DS,2,&workers,NULL)==INVALID_INPUT);
    assert(getCompanyWorkersByRank(DS,0,&workers,&numOfWorkers)==INVALID_INPUT);
    assert(getCompanyWorkersByRank(DS,-2,&workers,&numOfWorkers)==SUCCESS);
    //printf("OK1\n");
    assert(numOfWorkers==21);
    int arr[21]={1,109,108,107,106,105,104,103,102,101,100,69,68,67,66,65,20,24,23,22,21};
    for(int i=0;i<21;i++){
        assert(workers[i]==arr[i]);
    }
    free(workers);
    //printf("OK2\n");
    assert(getCompanyWorkersByRank(DS,2,&workers,&numOfWorkers)==SUCCESS);
    assert(numOfWorkers==0);
    free(workers);
    //printf("OK3\n");
    std::cout<<"Passed get company workers by rank test..."<<std::endl;
}


void testQuit(void* DS){
    void** tmp=NULL;
    void* temp=NULL;
    quit(tmp);
    quit(&temp);
    quit(&DS);
    assert(DS==NULL);
    std::cout<<"Passed Quit test..."<<std::endl;
}


int main(){
    void* DS=init();
    assert(DS);
    testAddCompany1(DS);
    //companies 1-5 were added
    testAddCompany2(DS);
    //compnies 100-109 were added
    testAddWorker(DS);
    testAddWorkerToCompany(DS);
    //worker 1 was added with no company,60-69 to company 1,100-109 to company 100-109,20-24 to company 3
    testRemoveWorker(DS);
    //workers 60-64 were removed
    testChangeRank(DS);
    testMergeCompanies(DS);
    testGetBestWorker(DS);
    testGetCompanyWorkersByRank(DS);
    testQuit(DS);
    return 0;
}