//
// Created by Tomer Golany on 10/08/2018.
//
/***************************************************************************/
/*                                                                         */
/* 234218 Data DSs 1, Summer 2018                                     */
/* Homework : Wet 1                                                        */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/* File Name : main1.cpp                                                   */
/*                                                                         */
/* Holds the "int main()" function and the parser of the shell's           */
/* command line.                                                           */
/***************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library1.h"
#include <iostream>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

/* The command's strings */
typedef enum {
	NONE_CMD = -2,
	COMMENT_CMD = -1,
	INIT_CMD = 0,
	ADDWORKER_CMD = 1,
	ADDCOMPANY_CMD = 2,
    ADDWORKERTOCOMPANY_CMD = 3,
	REMOVEWORKER_CMD = 4,
	MERGECOMPANIES_CMD = 5,
	CHANGERANK_CMD = 6,
	GETBESTWORKER_CMD = 7,
    GETCOMPANYWORKERSBYRANK_CMD = 8,
	QUIT_CMD = 9
} commandType;

static const int numActions = 9;
static const char *commandStr[] = {
		"init",
		"addWorker",
		"addCompany",
		"addWorkerToCompany",
        "removeWorker",
        "mergeCompanies",
        "changeRank",
		"getBestWorker",
		"getCompanyWorkersByRank ",
		"Quit" };

static const char* ReturnValToStr(int val) {
	switch (val) {
	case SUCCESS:
		return "SUCCESS";
	case ALLOCATION_ERROR:
		return "ALLOCATION_ERROR";
	case FAILURE:
		return "FAILURE";
	case INVALID_INPUT:
		return "INVALID_INPUT";
	default:
		return "";
	}
}

/* we assume maximum string size is not longer than 256  */
#define MAX_STRING_INPUT_SIZE (255)
#define MAX_BUFFER_SIZE       (255)

#define StrCmp(Src1,Src2) ( strncmp((Src1),(Src2),strlen(Src1)) == 0 )

typedef enum {
	error_free, error
} errorType;
static errorType parser(const char* const command);

#define ValidateRead(read_parameters,required_parameters,ErrorString,ErrorParams) \
if ( (read_parameters)!=(required_parameters) ) { printf(ErrorString, ErrorParams); return error; }

static bool isInit = false;

/***************************************************************************/
/* main                                                                    */
/***************************************************************************/

int main(int argc, const char**argv) {
	char buffer[MAX_STRING_INPUT_SIZE];

	// Reading commands
	while (fgets(buffer, MAX_STRING_INPUT_SIZE, stdin) != NULL) {
		fflush(stdout);
		if (parser(buffer) == error)
			break;
	};
	return 0;
}

/***************************************************************************/
/* Command Checker                                                         */
/***************************************************************************/

static commandType CheckCommand(const char* const command,
		const char** const command_arg) {
	if (command == NULL || strlen(command) == 0 || StrCmp("\n", command))
		return (NONE_CMD);
	if (StrCmp("#", command)) {
		if (strlen(command) > 1)
			printf("%s", command);
		return (COMMENT_CMD);
	};
	for (int index = 0; index < numActions; index++) {
		if (StrCmp(commandStr[index], command)) {
			*command_arg = command + strlen(commandStr[index]) + 1;
			return ((commandType) index);
		};
	};
	return (NONE_CMD);
}

/***************************************************************************/
/* Commands Functions                                                      */
/***************************************************************************/

static errorType OnInit(void** DS, const char* const command);
static errorType OnAddWorker(void* DS, const char* const command);
static errorType OnAddCompany(void* DS, const char* const command);
static errorType OnAddWorkerToCompany(void* DS, const char* const command);
static errorType OnRemoveWorker(void* DS, const char* const command);
static errorType OnMergeCompanies(void* DS, const char* const command);
static errorType OnChangeRank(void* DS, const char* const command);
static errorType OnGetBestWorker(void* DS, const char* const command);
static errorType OnGetCompanyWorkersByRank (void* DS, const char* const command);
static errorType OnQuit(void** DS, const char* const command);

/***************************************************************************/
/* Parser                                                                  */
/***************************************************************************/

static errorType parser(const char* const command) {
	static void *DS = NULL; /* The general data structure */
	const char* command_args = NULL;
	errorType rtn_val = error;

	commandType command_val = CheckCommand(command, &command_args);

	switch (command_val) {

	case (INIT_CMD):
		rtn_val = OnInit(&DS, command_args);
		break;
	case (ADDWORKER_CMD):
		rtn_val = OnAddWorker(DS, command_args);
		break;
	case (ADDCOMPANY_CMD):
		rtn_val = OnAddCompany(DS, command_args);
		break;
	case (ADDWORKERTOCOMPANY_CMD):
		rtn_val = OnAddWorkerToCompany(DS, command_args);
		break;
	case (REMOVEWORKER_CMD):
		rtn_val = OnRemoveWorker(DS, command_args);
		break;
	case (MERGECOMPANIES_CMD):
		rtn_val = OnMergeCompanies(DS, command_args);
		break;
	case (CHANGERANK_CMD):
		rtn_val = OnChangeRank(DS, command_args);
		break;
	case (GETBESTWORKER_CMD):
		rtn_val = OnGetBestWorker(DS, command_args);
		break;
    case (GETCOMPANYWORKERSBYRANK_CMD):
        rtn_val = OnGetCompanyWorkersByRank(DS, command_args);
        break;
	case (QUIT_CMD):
		rtn_val = OnQuit(&DS, command_args);
		break;

	case (COMMENT_CMD):
		rtn_val = error_free;
		break;
	case (NONE_CMD):
		rtn_val = error;
		break;
	default:
		assert(false);
		break;
	};
	return (rtn_val);
}

/***************************************************************************/
/* OnInit                                                                  */
/***************************************************************************/
static errorType OnInit(void** DS, const char* const command) {
	if (isInit) {
		printf("init was already called.\n");
		return (error_free);
	};
	isInit = true;

	*DS = init();
	if (*DS == NULL) {
		printf("init failed.\n");
		return error;
	};
	printf("init done.\n");

	return error_free;
}


/***************************************************************************/
/* OnAddWorker                                                             */
/***************************************************************************/
static errorType OnAddWorker(void* DS, const char* const command) {
	int workerID;
	int rank;
	ValidateRead(
			sscanf(command, "%d %d", &workerID, &rank),
			2, "%s failed.\n", commandStr[ADDWORKER_CMD]);
	StatusType res = addWorker(DS, workerID, rank);

	if (res != SUCCESS) {
		printf("%s: %s\n", commandStr[ADDWORKER_CMD], ReturnValToStr(res));
		return error_free;
	}

	printf("%s: %s\n", commandStr[ADDWORKER_CMD], ReturnValToStr(res));
	return error_free;
}

/***************************************************************************/
/* OnAddCompany                                                               */
/***************************************************************************/
static errorType OnAddCompany(void* DS, const char* const command) {
	int companyID;
	ValidateRead(sscanf(command, "%d", &companyID), 1, "%s failed.\n", commandStr[ADDCOMPANY_CMD]);
	StatusType res = addCompany(DS, companyID);

	if (res != SUCCESS) {
		printf("%s: %s\n", commandStr[ADDCOMPANY_CMD], ReturnValToStr(res));
		return error_free;
	} else {
		printf("%s: %s\n", commandStr[ADDCOMPANY_CMD], ReturnValToStr(res));
	}

	return error_free;
}


/***************************************************************************/
/* OnAddWorkerToCompany                                                              */
/***************************************************************************/
static errorType OnAddWorkerToCompany(void* DS, const char* const command) {
	int workerID, companyID;
	ValidateRead(sscanf(command, "%d %d", &workerID, &companyID), 2,
			"%s failed.\n", commandStr[ADDWORKERTOCOMPANY_CMD]);
	StatusType res = addworkerToCompany(DS, workerID, companyID);
	if (res != SUCCESS) {
		printf("%s: %s\n", commandStr[ADDWORKERTOCOMPANY_CMD], ReturnValToStr(res));
		return error_free;
	}

	printf("%s: %s\n", commandStr[ADDWORKERTOCOMPANY_CMD], ReturnValToStr(res));
	return error_free;
}

/***************************************************************************/
/* OnRemoveWorker                                                     */
/***************************************************************************/
static errorType OnRemoveWorker(void* DS, const char* const command) {
	int workerID;
	ValidateRead(sscanf(command, "%d", &workerID), 1,
			"%s failed.\n", commandStr[REMOVEWORKER_CMD]);
	StatusType res = removeWorker(DS, workerID);

	if (res != SUCCESS) {
		printf("%s: %s\n", commandStr[REMOVEWORKER_CMD], ReturnValToStr(res));
		return error_free;
	}

	printf("%s: %s\n", commandStr[REMOVEWORKER_CMD], ReturnValToStr(res));
	return error_free;
}

/***************************************************************************/
/* OnMergeCompanies                                                            */
/***************************************************************************/
static errorType OnMergeCompanies(void* DS, const char* const command) {
	int company1;
	int company2;
    int minRank;
	ValidateRead(sscanf(command, "%d %d %d", &company1, &company2, &minRank), 3,
			"%s failed.\n", commandStr[MERGECOMPANIES_CMD]);
	StatusType res = mergeCompanies(DS, company1, company2, minRank);

	if (res != SUCCESS) {
		printf("%s: %s\n", commandStr[MERGECOMPANIES_CMD], ReturnValToStr(res));
		return error_free;
	}

	printf("%s: %s\n", commandStr[MERGECOMPANIES_CMD], ReturnValToStr(res));
	return error_free;
}


/***************************************************************************/
/* OnChangeRank                                                            */
/***************************************************************************/
static errorType OnChangeRank(void* DS, const char* const command) {
    int workerID;
    int newRank;
    ValidateRead(sscanf(command, "%d %d", &workerID, &newRank), 2,
                 "%s failed.\n", commandStr[CHANGERANK_CMD]);
    StatusType res = changeRank(DS, workerID, newRank);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[CHANGERANK_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[CHANGERANK_CMD], ReturnValToStr(res));
    return error_free;
}


/***************************************************************************/
/* OnGetBestWorker                                                         */
/***************************************************************************/
static errorType OnGetBestWorker(void* DS, const char* const command) {
	int companyID;
	ValidateRead(sscanf(command, "%d", &companyID), 1, "%s failed.\n", commandStr[GETBESTWORKER_CMD]);
	int workerID;
	StatusType res = getBestWorker(DS, companyID, &workerID);

	if (res != SUCCESS) {
		printf("%s: %s\n", commandStr[GETBESTWORKER_CMD], ReturnValToStr(res));
		return error_free;
	}

	cout << "Most successful worker is: " << workerID << endl;
	return error_free;
}


/***************************************************************************/
/* PrintAll                                                                */
/***************************************************************************/
void PrintAll(int *workerIDs, int numOfWorkers) {
	if (numOfWorkers > 0) {
		cout << "Rank\t||\tworker" << endl;
	}

	for (int i = 0; i < numOfWorkers; i++) {
		cout << i + 1 << "\t\t||\t" << workerIDs[i] << endl;
	}
	cout << "and there are no more workers!" << endl;

	free (workerIDs);
}

static errorType OnGetCompanyWorkersByRank(void* DS, const char* const command) {
	int companyID;
	ValidateRead(sscanf(command, "%d", &companyID), 1,
			"%s failed.\n", commandStr[GETCOMPANYWORKERSBYRANK_CMD]);
	int* workerIDs;
	int numOfWorkers;
	StatusType res = getCompanyWorkersByRank(DS, companyID, &workerIDs, &numOfWorkers);

	if (res != SUCCESS) {
		printf("%s: %s\n", commandStr[GETCOMPANYWORKERSBYRANK_CMD], ReturnValToStr(res));
		return error_free;
	}

	PrintAll(workerIDs, numOfWorkers);
	return error_free;
}

/***************************************************************************/
/* OnQuit                                                                  */
/***************************************************************************/
static errorType OnQuit(void** DS, const char* const command) {
	quit(DS);
	if (*DS != NULL) {
		printf("Quit failed.\n");
		return error;
	};

	isInit = false;
	printf("Quit done.\n");

	return error_free;
}

#ifdef __cplusplus
}
#endif
