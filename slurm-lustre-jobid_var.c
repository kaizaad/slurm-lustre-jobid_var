/******************************************************************************\
 * slurm-lustre-jobid_var.c - SLURM SPANK plugin Lustre jobid_var creator
 *******************************************************************************
 * This creates a new environment variable which is a concatenation of 
 * SLURM_JOB_ID.SLURM_JOB_UID.SLURM_JOB_ACCOUNT that can be used as the Lustre
 * jobid_var environment variable. By using the SLURM SPANK Plugin API we are 
 * able to have this variable be visible in the running job's environment.
 * The reason for having a combined variable is to provide quicker searching
 * abilities within the Elasticsearch documents where the Lustre jobstat metrics
 * end up being stored and analyzed.
\******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
 * The maximum size of the environment variable used by lustre job_stats is 31
 * usable bytes + null terminator.
 */
#define MAXSIZE 32

int main(int argc, char** argv)
{

	char slurm_tuplet[MAXSIZE] = "";
	char *slurm_job_id = NULL;
	char *slurm_job_uid = NULL;
	char *slurm_job_account = NULL;
	size_t sbytes_leftover;


	// Get the SLURM environment variables we are interested in
	slurm_job_id = getenv("SLURM_JOB_ID");
	slurm_job_uid = getenv("SLURM_JOB_UID");
	slurm_job_account = getenv("SLURM_JOB_ACCOUNT");
	
	/* 
	 * Concatenate the environment strings making sure we don't exceed MAXSIZE
	 * The strlen() + delimiter should be < MAXSIZE - NULL terminator
	 */
	if (slurm_job_id == NULL || (strlen(slurm_job_id) + 1 > MAXSIZE - 1)) {
		fprintf(stderr,"SLURM_JOB_ID is either NULL or too long to use!\n");
		return (EXIT_FAILURE);
	} else {
		strcpy(slurm_tuplet,slurm_job_id);
		strcat(slurm_tuplet, ".");
	}

	if (slurm_job_uid == NULL || (strlen(slurm_tuplet) + strlen(slurm_job_uid) + 1 > MAXSIZE - 1)) {
		fprintf(stderr,"SLURM_JOB_UID is either NULL or too long to use!\n");
		return (EXIT_FAILURE);
	} else {
		strcat(slurm_tuplet, slurm_job_uid);
		strcat(slurm_tuplet, ".");
	}

	// Get the # of bytes to add to keep slurm_tuplet <= MAXSIZE - 1
	if (slurm_job_account == NULL) {
		fprintf(stderr,"SLURM_JOB_ACCOUNT is NULL!\n");
        return (EXIT_FAILURE);
    } else {
		sbytes_leftover = (MAXSIZE - 1) - (strlen(slurm_tuplet));
		strncat(slurm_tuplet,slurm_job_account,sbytes_leftover);
	}

	fprintf(stdout,"SLURM_TUPLET is : %s\n", slurm_tuplet);
	fprintf(stdout,"SLURM_TUPLET lenght is : %lu\n", strlen(slurm_tuplet));

	setenv("SLURM_TUPLET",slurm_tuplet,0);
	fprintf(stdout,"The SLURM_TUPLET from environment is : %s\n", getenv("SLURM_TUPLET"));

	return(0);

}
