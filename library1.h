//
// Created by Tomer Golany on 10/08/2018.
//
/****************************************************************************/
/*                                                                          */
/* This file contains the interface functions                               */
/* you should use for the wet ex 1                                          */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : library1.h                                                   */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_
#define _234218_WET1_

#ifdef __cplusplus
extern "C" {
#endif

/* Return Values
 * ----------------------------------- */
typedef enum {
    SUCCESS = 0,
    FAILURE = -1,
    ALLOCATION_ERROR = -2,
    INVALID_INPUT = -3
} StatusType;

/* Required Interface for the Data Structure
 * -----------------------------------------*/

/* Description:   Initiates the data structure.
 * Input:         DS - A pointer to the data structure.
 * Output:        None.
 * Return Values: A pointer to a new instance of the data structure - as a void* pointer.
 */
void* init();

/* Description:   Adds a new empty worker to the system.
 * Input:         DS - A pointer to the data structure.
 *                workerID - The new worker's ID.
 *                rank - worker's rank.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, or workerID <= 0 or rank <= 0 .
 *                FAILURE - If workerID is already in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType addWorker(void *DS, int workerID, int rank);

/* Description:   Adds a new company to the system.
 * Input:         DS - A pointer to the data structure.
 *                companyID - The ID of the new company.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, companyID <= 0.
 *                FAILURE - If companyID is already in the system.
 *                SUCCESS - Otherwise.
 */
StatusType addCompany(void *DS, int companyID);

/* Description:   Moves a worker to a company.
 * Input:         DS - A pointer to the data structure.
 *                companyID - The ID of the company.
 *                workerID - The IDd of the worker
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, companyID <= 0, workerID <= 0.
 *                FAILURE - If companyID doesn't exist or if wokerId doesn't exist.
 *                SUCCESS - Otherwise.
 */
StatusType addworkerToCompany(void *DS, int workerID, int companyID);

/* Description:   removes a worker from the system.
 * Input:         DS - A pointer to the data structure.
 *                workerID - The ID of the worker
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, workerID <= 0.
 *                FAILURE - If workerID doesn't exist.
 *                SUCCESS - Otherwise.
 */
StatusType removeWorker(void *DS, int workerID);


/* Description:   Merge to companies, leaving the workers below minimalRank out of the merged company.
 * Input:         DS - A pointer to the data structure.
 *                companyID1 - ID of the first company.
 *		          companyID2 - ID of the second company.
 *		          minimalRank - minimal rank.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, or if companyID1<=0, or if companyID2<=0 or if minimalRank <= 0 or if
 *                companyID1 == companyID2
 *                FAILURE - If companyID1 or companyID2 are not in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType mergeCompanies(void *DS, int companyID1, int companyID2, int minimalRank);

/* Description:   Change rank to workerID
 * Input:         DS - A pointer to the data structure.
 *                workerID - The worker to change rank
 *                newRank - The new rank for the worker.
 * Output:        None.
 * Return Values: INVALID_INPUT - If DS==NULL, or if workerID <= 0, or if newRank <= 0.
 *                FAILURE - if workerID > 0 and WorkerID doesn't exist in the DS
 *                SUCCESS - Otherwise.
 */
StatusType changeRank(void *DS, int workerID, int newRank);

/* Description:   Returns the worker that has the best rank in companyID
 * 			      If companyID < 0, returns the worker that has the best rank in the entire DS.
 * Input:         DS - A pointer to the data structure.
 *                companyID - The company that we would like to get the data for.
 * Output:        workerID - A pointer to a variable that should be updated to the ID of the worker with the most completed challanges.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, or if workerID == NULL, or if companyID == 0.
 *                FAILURE - If companyID > 0 and company doesn't exist in the DS
 *                SUCCESS - Otherwise.
 */
StatusType getBestWorker(void *DS, int companyID, int *workerID);

/* Description:   Returns all the workers in company companyID sorted by their rank.
 *           	  If companyID < 0, returns all the workers in the entire DS sorted by their rank.
 * Input:         DS - A pointer to the data structure.
 *                companyID - The company that we would like to get the data for.
 * Output:        workers - A pointer to a to an array that you should update with the workers' IDs sorted by their rank,
 *			                  in case two workers have same rank, they should be sorted by their ID.
 *                numOfWorkers - A pointer to a variable that should be updated to the number of returned workers.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If any of the arguments is NULL or if companyID == 0.
 *                FAILURE - if companyID > 0 and companyID doesn't exist in the DS
 *                SUCCESS - Otherwise.
 */
StatusType getCompanyWorkersByRank (void *DS, int companyID, int **workers, int *numOfWorkers);

/* Description:   Quits and deletes the database.
 *                DS should be set to NULL.
 * Input:         DS - A pointer to the data structure.
 * Output:        None.
 * Return Values: None.
 */
void quit(void** DS);

#ifdef __cplusplus
}
#endif

#endif    /*_234218_WET1_ */
