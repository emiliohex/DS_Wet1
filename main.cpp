
#include <iostream>
#include "library1.h"
#include "test_utilities.h"
void David_test(){
    cout << "-------------------------------------init test-------------------------------------" << endl;
    void *DS = init();
    test(DS != NULL, "init failed", __LINE__);
    cout << "-----------------------------------addWorker test-----------------------------------" << endl;
    test(addWorker(DS, 1, 1) == SUCCESS, "addWorkerFailed", __LINE__);
    test(addWorker(DS, 2, 1) == SUCCESS, "addWorkerFailed", __LINE__);
    test(addWorker(DS, 3, 2) == SUCCESS, "addWorkerFailed", __LINE__);
    test(addWorker(DS, 4, 1) == SUCCESS, "addWorkerFailed", __LINE__);
    test(addWorker(DS, 4, 1) == FAILURE, "addWorker didn't Fail - Worker exists", __LINE__);
    test(addWorker(DS, 4, 2) == FAILURE, "addWorker didn't Fail - Worker exists- diffrent rank", __LINE__);
    test(addWorker(DS, 5, -1) == INVALID_INPUT, "addWorker didn't Fail - Invalid Info - invalid rank", __LINE__);
    test(addWorker(DS, 5, 0) == INVALID_INPUT, "addWorker didn't Fail - Invalid Info - invalid rank", __LINE__);
    test(addWorker(NULL, 5, 2) == INVALID_INPUT, "addWorker didn't Fail - Invalid Info - NULL DS", __LINE__);
    test(addWorker(DS, 0, 2) == INVALID_INPUT, "addWorker didn't Fail - Invalid Info - invalid worder id", __LINE__);
    test(addWorker(DS, -1, 2) == INVALID_INPUT, "addWorker didn't Fail - Invalid Info - invalid worder id", __LINE__);
    for (int i = 5; i < 15; i++) {
        test(addWorker(DS, i, 5) == SUCCESS, "addWorkerFailed", __LINE__);
    }
    for (int i = 15; i < 20; i++)
        test(addWorker(DS, i, 8) == SUCCESS, "addWorkerFailed", __LINE__);
    //inOrderPrint(DS);
    for (int i = 150; i < 160; i++)
        test(addWorker(DS, i, 8) == SUCCESS, "addWorkerFailed", __LINE__);
    cout << "-----------------------------------addCompany test-----------------------------------" << endl;
    test(addCompany(DS, 1) == SUCCESS, "addCompanyFailed", __LINE__);
    test(addCompany(DS, 2) == SUCCESS, "addCompanyFailed", __LINE__);
    test(addCompany(DS, 3) == SUCCESS, "addCompanyFailed", __LINE__);
    test(addCompany(DS, 4) == SUCCESS, "addCompanyFailed", __LINE__);
    test(addCompany(DS, 1) == FAILURE, "addCompany didn't Fail - Company exists", __LINE__);
    test(addCompany(DS, 0) == INVALID_INPUT, "addCompany didn't Fail - Invalid input - company id", __LINE__);
    test(addCompany(DS, -21) == INVALID_INPUT, "addCompany didn't Fail - Invalid input - company id", __LINE__);
    test(addCompany(NULL, 1) == INVALID_INPUT, "addCompany didn't Fail - Invalid input - NULL ptr", __LINE__);
    //inOrderPrint(DS);
    for (int i = 150; i < 160; i++)
        test(addCompany(DS, i) == SUCCESS, "addCompanyFailed", __LINE__);
    cout << "---------------------------------addWorkerToCompany test---------------------------------" << endl;
    test(addworkerToCompany(DS, 0, 3) == INVALID_INPUT, "addWorkerToCompany didn't Fail - Invalid input- worker",
         __LINE__);
    test(addworkerToCompany(DS, 4, 0) == INVALID_INPUT, "addWorkerToCompany didn't Fail - Invalid input- company",
         __LINE__);
    test(addworkerToCompany(DS, -1, 3) == INVALID_INPUT, "addWorkerToCompany didn't Fail - Invalid input- worker",
         __LINE__);
    test(addworkerToCompany(DS, 4, -1) == INVALID_INPUT, "addWorkerToCompany didn't Fail - Invalid input- company",
         __LINE__);
    test(addworkerToCompany(NULL, 2, 3) == INVALID_INPUT, "addWorkerToCompany didn't Fail - Invalid input- null",
         __LINE__);
    test(addworkerToCompany(DS, 2, 20) == FAILURE, "addWorkerToCompany didn't Fail - Worker doesn't exits", __LINE__);
    test(addworkerToCompany(DS, 5, 7) == FAILURE, "addWorkerToCompany didn't Fail - Company doesn't exits", __LINE__);
    test(addworkerToCompany(DS, 1, 1) == SUCCESS, "addWorkerToCompany Failed", __LINE__);
    test(addworkerToCompany(DS, 2, 2) == SUCCESS, "addWorkerToCompany Failed", __LINE__);
    test(addworkerToCompany(DS, 3, 3) == SUCCESS, "addWorkerToCompany Failed", __LINE__);
    for (int j = 4; j < 15; ++j) {
        test(addworkerToCompany(DS, j, 3) == SUCCESS, "addWorkerToCompany Failed", __LINE__);
    }
    for (int j = 15; j < 20; ++j) {
        test(addworkerToCompany(DS, j, 4) == SUCCESS, "addWorkerToCompany Failed", __LINE__);
    }
    // company 1- has worker 1
    // company 2-2
    // company 3-   3-14
    // company 4-   15-19
    cout << "---------------------------------removeWorker test---------------------------------" << endl;
    test(removeWorker(NULL, 14) == INVALID_INPUT, "addWorkerToCompany Didnt Fail-bad input-NULL", __LINE__);
    test(removeWorker(DS, 0) == INVALID_INPUT, "addWorkerToCompany Didnt Fail-bad input- worker", __LINE__);
    test(removeWorker(DS, -1) == INVALID_INPUT, "addWorkerToCompany Didnt Fail-bad input -worker", __LINE__);
   test(removeWorker(DS, 14) == SUCCESS, "addWorkerToCompany Failed", __LINE__);//changes 15 pointer!!!!!!!
   test(removeWorker(DS, 14) == FAILURE, "addWorkerToCompany Didnt Fail - worker doesnt exist", __LINE__);
    cout << "---------------------------------mergeCompanies test---------------------------------" << endl;
    test(mergeCompanies(NULL, 1, 2, 3) == INVALID_INPUT, "mergeCompanies Didnt Fail-bad input-NULL", __LINE__);
    test(mergeCompanies(DS, 1, 1, 3) == INVALID_INPUT, "mergeCompanies Didnt Fail-bad input-same company", __LINE__);
    test(mergeCompanies(DS, 0, 2, 0) == INVALID_INPUT, "mergeCompanies Didnt Fail-bad input-bad company input",
         __LINE__);
    test(mergeCompanies(DS, -1, 0, 3) == INVALID_INPUT, "mergeCompanies Didnt Fail-bad input-bad company input",
         __LINE__);
    test(mergeCompanies(DS, 1, -1, 3) == INVALID_INPUT, "mergeCompanies Didnt Fail-bad input-bad company input",
         __LINE__);
    test(mergeCompanies(DS, 1, 0, 3) == INVALID_INPUT, "mergeCompanies Didnt Fail-bad input-bad company input",
         __LINE__);
    test(mergeCompanies(DS, 1, 2, -1) == INVALID_INPUT, "mergeCompanies Didnt Fail-bad input-bad rank", __LINE__);
    test(mergeCompanies(DS, 1, 2, 0) == INVALID_INPUT, "mergeCompanies Didnt Fail-bad input-bad rank", __LINE__);
    test(mergeCompanies(DS, 1, 4,3) == SUCCESS, "mergeCompanies Didnt Fail-bad input-compay doesnt exist", __LINE__);
   test(changeRank(DS,15,10)==SUCCESS,"change rank failed after merge",__LINE__);
    test(changeRank(DS,16,1)==SUCCESS,"change rank failed after merge",__LINE__);
    test(changeRank(DS,17,2)==SUCCESS,"change rank failed after merge",__LINE__);
    test(changeRank(DS,18,2)==SUCCESS,"change rank failed after merge",__LINE__);
    test(changeRank(DS,19,2)==SUCCESS,"change rank failed after merge",__LINE__);
    test(changeRank(DS,1,1)==SUCCESS,"change rank failed after merge",__LINE__);
    test(mergeCompanies(DS, 2, 4, 4) == SUCCESS, "mergeCompanies Failed", __LINE__);
    test(mergeCompanies(DS, 1, 2, 1) == FAILURE, "mergeCompanies Didnt Fail-bad input-should exist after merge",
         __LINE__);
    test(mergeCompanies(DS, 4, 3, 4) == SUCCESS, "mergeCompanies Failed", __LINE__);
    cout << "---------------------------------changeRank test---------------------------------" << endl;
    test(changeRank(NULL, 1, 10) == INVALID_INPUT, "changeRank Didnt Fail-bad input-NULL", __LINE__);
    test(changeRank(DS, 0, 10) == INVALID_INPUT, "changeRank Didnt Fail-bad input-workerID", __LINE__);
    test(changeRank(DS, -1, 10) == INVALID_INPUT, "changeRank Didnt Fail-bad input-workerID", __LINE__);
    test(changeRank(DS, 1, 0) == INVALID_INPUT, "changeRank Didnt Fail-bad input-rank", __LINE__);
    test(changeRank(DS, 1, -1) == INVALID_INPUT, "changeRank Didnt Fail-bad input-rank", __LINE__);
    test(changeRank(DS, 25, 10) == FAILURE, "changeRank Didnt Fail-bad input-worker doesnt exist", __LINE__);
    test(changeRank(DS, 1, 10) == SUCCESS, "changeRank Didnt Fail-bad input-worker doesnt exist", __LINE__);
    cout << "---------------------------------getBestWorker test---------------------------------" << endl;
    int workerNum = 0;
    test(getBestWorker(NULL, 1, &workerNum) == INVALID_INPUT, "getBestWorker Didnt Fail-bad input-NULL", __LINE__);
    test(getBestWorker(DS, 1, NULL) == INVALID_INPUT, "getBestWorker Didnt Fail-bad input-NULL", __LINE__);
    test(getBestWorker(DS, 0, &workerNum) == INVALID_INPUT, "getBestWorker Didnt Fail-bad input-company id",
         __LINE__);
    test(getBestWorker(DS, 10, &workerNum) == FAILURE, "getBestWorker Didnt Fail-bad input-company doesnt exist",
         __LINE__);
    test(((getBestWorker(DS, -1, &workerNum) == SUCCESS) && workerNum == 1), "getBestWorker - bad output",
         __LINE__);
    test(((getBestWorker(DS, 3, &workerNum) == SUCCESS) && workerNum == 15), "getBestWorker - bad output",
         __LINE__);
    test(addworkerToCompany(DS, 1,3)==SUCCESS,"addWorkerToCompany failed",__LINE__);
    test(((getBestWorker(DS, 3, &workerNum) == SUCCESS) && workerNum == 1), "getBestWorker - bad output",
         __LINE__);
    quit(&DS);
    test(DS==NULL, "Quit didn't put NULL at DS ", __LINE__);

}
void leaktests() {
    cout << "-------------------------------------init test-------------------------------------" << endl;
    void *DS = init();
///add new workers
    for (int i = 1; i < 20; i++) {
        test(addWorker(DS, i, 5 + (i % 3)) == SUCCESS, "addWorkerFailed", __LINE__);
    }
    test(addCompany(DS, 1) == SUCCESS, "addCompanyFailed", __LINE__);
    test(addCompany(DS, 2) == SUCCESS, "addCompanyFailed", __LINE__);

    for (int j = 1; j < 20; ++j) {
        test(addworkerToCompany(DS, j, 1) == SUCCESS, "addWorkerToCompany Failed", __LINE__);
    }
    for (int j = 10; j < 20; ++j) {
        if(j==18){
            std::cout<<"hi"<<std::endl;
        }
        test(addworkerToCompany(DS, j, 2) == SUCCESS, "addWorkerToCompany Failed", __LINE__);
    }
    ///company 1: 1-9
    ///company 2: 10-19
    test(mergeCompanies(DS, 1, 2,6) == SUCCESS, "mergeCompanies Didnt Fail-bad input-compay doesnt exist", __LINE__);
    ///company1 has all workers
    test(changeRank(DS,1,100)==SUCCESS,"change rank failed after merge",__LINE__);
    test(changeRank(DS,2,100)==SUCCESS,"change rank failed after merge",__LINE__);
    test(changeRank(DS,18,100)==SUCCESS,"change rank failed after merge",__LINE__);
    test(changeRank(DS,19,100)==SUCCESS,"change rank failed after merge",__LINE__);
    quit(&DS);
}

int main() {
    cout<<"-------------------------------------init test-------------------------------------"<<endl;
    void* DS=init();
    test(DS!=NULL,"init failed",__LINE__);
    cout<<"-----------------------------------addWorker test-----------------------------------"<<endl;
    test(addWorker(DS,4,1)==SUCCESS,"addWorkerFailed",__LINE__);
    test(addWorker(DS,6,1)==SUCCESS,"addWorkerFailed",__LINE__);
    test(addWorker(DS,7,2)==SUCCESS,"addWorkerFailed",__LINE__);
    test(addWorker(DS,4,1)==FAILURE,"addWorker didn't Fail - Worker exists",__LINE__);
    test(addWorker(DS,3,1)==SUCCESS,"addWorkerFailed",__LINE__);
    test(addWorker(DS,4,0)==INVALID_INPUT,"addWorker didn't Fail - Invalid Info",__LINE__);
    for(int i=8;i<14;i++)
    test(addWorker(DS,i,1)==SUCCESS,"addWorkerFailed",__LINE__);
    cout<<"-----------------------------------addCompany test-----------------------------------"<<endl;
    test(addCompany(DS,1)==SUCCESS,"addCompanyFailed",__LINE__);
    test(addCompany(DS,3)==SUCCESS,"addCompanyFailed",__LINE__);
    test(addCompany(DS,1)==FAILURE,"addCompany didn't Fail - Company exists",__LINE__);
    test(addCompany(DS,0)==INVALID_INPUT,"addCompany didn't Fail - Invalid input",__LINE__);
    test(addCompany(DS,-21)==INVALID_INPUT,"addCompany didn't Fail - Invalid input",__LINE__);
    test(addCompany(NULL,1)==INVALID_INPUT,"addCompany didn't Fail - Invalid input",__LINE__);
    cout<<"---------------------------------addWorkerToCompany test---------------------------------"<<endl;
    test(addworkerToCompany(DS,3,3)==SUCCESS,"addWorkerToCompany Failed",__LINE__);
    test(addworkerToCompany(DS,0,3)==INVALID_INPUT,"addWorkerToCompany didn't Fail - Invalid input",145);
    test(addworkerToCompany(DS,2,3)==FAILURE,"addWorkerToCompany didn't Fail - Worker doesn't exsits",146);
    test(addworkerToCompany(DS,3,7)==FAILURE,"addWorkerToCompany didn't Fail - Company doesn't exsits",147);
    test(addworkerToCompany(NULL,2,3)==INVALID_INPUT,"addWorkerToCompany didn't Fail - Invalid input",148);
    test(addworkerToCompany(DS,7,3)==SUCCESS,"addWorkerToCompany Failed",__LINE__);
    test(addworkerToCompany(DS,7,1)==SUCCESS,"addWorkerToCompany Failed",__LINE__);
    test(addworkerToCompany(DS,12,3)==SUCCESS,"addWorkerToCompany Failed",__LINE__);
    cout<<"---------------------------------removeWorker test---------------------------------"<<endl;
    test(removeWorker(DS,7)==SUCCESS,"addWorkerToCompany Failed",__LINE__);
    test(removeWorker(DS,7)==FAILURE,"addWorkerToCompany didn't fail - Worker doesn't exsits",__LINE__);
    test(removeWorker(DS,3)==SUCCESS,"addWorkerToCompany Failed",__LINE__);
    cout<<"---------------------------------changeRank test---------------------------------"<<endl;
    test(changeRank(DS,13,5)==SUCCESS,"changeRank Failed",__LINE__);
    test(changeRank(DS,12,5)==SUCCESS,"changeRank Failed",__LINE__);
    test(changeRank(DS,12,1)==SUCCESS,"changeRank Failed",__LINE__);
    cout<<"---------------------------------getBestWorker test---------------------------------"<<endl;
    int workerID;
    test(getBestWorker(DS,3,&workerID)==SUCCESS,"getBestWorker Failed",__LINE__);
    test(workerID==12,"Wrong best worker from company ID 3",__LINE__);
    test(getBestWorker(DS,-1,&workerID)==SUCCESS,"getBestWorker Failed",__LINE__);
    test(workerID==13,"Wrong best worker from general workers",__LINE__);
    test(getBestWorker(DS,5,&workerID)==FAILURE,"getBestWorker didn't fail - Company doesn't exsits",__LINE__);
    test(getBestWorker(DS,3,NULL)==INVALID_INPUT,"getBestWorker didn't fail - NULL pointer",__LINE__);
    test(getBestWorker(NULL,3,&workerID)==INVALID_INPUT,"getBestWorker didn't fail - NULL pointer",__LINE__);
    test(getBestWorker(DS,0,&workerID)==INVALID_INPUT,"getBestWorker didn't fail - companyID 0",__LINE__);
    test(getBestWorker(DS,1,&workerID)==SUCCESS,"getBestWorker didn't fail - companyID 0",__LINE__);
    test(workerID==-1,"Wrong best worker from company ID 1",__LINE__);
    cout<<"---------------------------------getCompanyWorkersByRank test---------------------------------"<<endl;
    int* workers;int num;
    test(getCompanyWorkersByRank(DS,-1,&workers,&num)==SUCCESS,"getCompanyWorkersByRank general didn't work",__LINE__);
    test(num==8,"getCompanyWorkersByRank - wrong number of workers",__LINE__);
    test(workers[0]==13,"getCompanyWorkersByRank - wrong array",__LINE__);
    test(workers[1]==4,"getCompanyWorkersByRank - wrong array",__LINE__);
    test(workers[2]==6,"getCompanyWorkersByRank - wrong array",__LINE__);
    test(workers[3]==8,"getCompanyWorkersByRank - wrong array",__LINE__);
    test(workers[4]==9,"getCompanyWorkersByRank - wrong array",__LINE__);
    test(workers[5]==10,"getCompanyWorkersByRank - wrong array",__LINE__);
    test(workers[6]==11,"getCompanyWorkersByRank - wrong array",__LINE__);
    test(workers[7]==12,"getCompanyWorkersByRank - wrong array",__LINE__);
    free(workers);
    test(getCompanyWorkersByRank(DS,0,&workers,&num)==INVALID_INPUT,"getCompanyWorkersByRank didn't fail",__LINE__);
    test(getCompanyWorkersByRank(DS,1,&workers,&num)==SUCCESS,"getCompanyWorkersByRank general didn't work",__LINE__);
    test(num==0,"getCompanyWorkersByRank - wrong number of workers",__LINE__);
    test(workers==NULL,"getCompanyWorkersByRank - needed empty array",__LINE__);
    test(getCompanyWorkersByRank(DS,3,&workers,&num)==SUCCESS,"getCompanyWorkersByRank general didn't work",__LINE__);
    test(num==1,"getCompanyWorkersByRank - wrong number of workers",__LINE__);
    test(workers[0]==12,"getCompanyWorkersByRank - wrong array",__LINE__);
    free(workers);
    test(getCompanyWorkersByRank(DS,33,&workers,&num)==FAILURE,"getCompanyWorkersByRank - No such company",__LINE__);
    cout<<"---------------------------------mergeCompanies test---------------------------------"<<endl;
    test(addworkerToCompany(DS,11,1)==SUCCESS,"addWorkerToCompany failed",__LINE__);
    test(mergeCompanies(DS,1,3,1)==SUCCESS,"merge failed",__LINE__);
    test(getBestWorker(DS,1,&num)==SUCCESS,"bestWorker failed",__LINE__);
    test(num==11,"bestWorker after merge failed",__LINE__);
    test(addCompany(DS,3)==SUCCESS,"addCompany failed",__LINE__);
    test(addworkerToCompany(DS,13,3)==SUCCESS,"addWorkerToCompany failed",__LINE__);
    test(mergeCompanies(DS,1,3,3)==SUCCESS,"merge failed",__LINE__);
    test(getBestWorker(DS,1,&num)==SUCCESS,"bestWorker failed",__LINE__);
    test(num==13,"bestWorker after merge failed",__LINE__);
    changeRank(DS,13,11);//will fail if pointers from General IDAVL wont point to company's AVLs
    test(getCompanyWorkersByRank(DS,1,&workers,&num)==SUCCESS,"getComapnyWorkersByRank failed",__LINE__);
    test(num==1,"number of workers at company1 after merge is wrong - need to be 1",__LINE__);
    free (workers);
    for (int i = 3200; i >=3000; i--)
         test(addWorker(DS, i, 3) == SUCCESS, "addWorker not perfect", __LINE__);
    for (int i = 3000; i <3200; i++)
        test(addCompany(DS, i) == SUCCESS, "addCompany not perfect", __LINE__);
    for(int i=3000;i<=3100;i++){
        test(addworkerToCompany(DS,i,1)==SUCCESS,"addWorkerToCompany not perfect",__LINE__);
    }
    for(int i=3000;i<=3200;i++){
        test(changeRank(DS,i,i)==SUCCESS,"changeRank not perfect",__LINE__);
        test(getBestWorker(DS,-1,&num)==SUCCESS,"getBestWorker general not perfect",__LINE__);
        test(num==i,"bestWorker general gives false data",__LINE__);
    }
    test(getCompanyWorkersByRank(DS,-1,&workers,&num)==SUCCESS,"getCompanyWorkersByRank general - didn't work",__LINE__);
    for(int i=3000;i<=3200;i++){
        test(workers[i-3000]==3200-i+3000,"getBestWorkersByRank general - wrong array",__LINE__);
    }
    free (workers);
    test(getCompanyWorkersByRank(DS,1,&workers,&num)==SUCCESS,"getCompanyWorkersByRank of companyID 1 - didn't work",__LINE__);
    for(int i=3000;i<=3100;i++){
        test(workers[i-3000]==3100-i+3000,"getBestWorkersByRank of companyID 1 - wrong array",__LINE__);
    }
    free (workers);
    for(int i=3000;i<=3200;i++){
        test(removeWorker(DS,i)==SUCCESS,"removeWorker not perfect",__LINE__);
    }
    cout<<"---------------------------------Quit test---------------------------------"<<endl;
    quit(&DS);
    test(DS==NULL,"Quit failed - DS!=NULL",__LINE__);
    David_test();
    leaktests();
    print_grade();
    return 0;
}