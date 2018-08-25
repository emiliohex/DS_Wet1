#include <iostream>
#include "library1.h"
#include <cassert>
#include <stdlib.h>

int main(){
    printf("init\n");
    void* DS=init();
    printf("addWorker 1 0 %d\n",addWorker(DS,1,0));
    printf("addWorker 1 2 %d\n",addWorker(DS,1,2));
    printf("addWorker 2 2 %d\n",addWorker(DS,2,2));
    printf("addCompany 1 %d\n",addCompany(DS,1));
    printf("addworkerToCompany 1 1 %d\n",addworkerToCompany(DS,1,1));
    printf("mergeCompanies 1 1 0 %d\n",mergeCompanies(DS,1,1,0));
    printf("changeRank 1 5 %d\n",changeRank(DS,1,5));
    int best = -1;
    printf("addCompany 2 %d\n",addCompany(DS,2));
    printf("addworkerToCompany 1 2 %d\n",addworkerToCompany(DS,1,2));
    printf("getBestWorker 2 %d\n",getBestWorker(DS,2,&best));
    printf("bestWorker - %d\n",best);
    printf("getBestWorker 1 %d\n",getBestWorker(DS,1,&best));
    printf("bestWorker - %d\n",best);
    printf("addCompany 2 %d\n",addCompany(DS,2));
    printf("addworkerToCompany 2 2 %d\n",addworkerToCompany(DS,2,2));
    printf("mergeCompanies 2 1 1 %d\n",mergeCompanies(DS,2,1,1));
    printf("changeRank 2 7 %d\n",changeRank(DS,2,7));
    printf("getBestWorker 1 %d\n",getBestWorker(DS,1,&best));
    printf("bestWorker - %d\n",best);
    int numOfWorkers=-1;
    int *workers=&numOfWorkers;
    printf("getCompanyWorkersByRank 1 %d\n",getCompanyWorkersByRank(DS,1,&workers,&numOfWorkers));
    for(int i=0;i<numOfWorkers;i++){
        printf("workers[%d] - %d\n",i,workers[i]);
    }
    free(workers);
    printf("QUIT\n");
    quit(&DS);
    return 0;
}