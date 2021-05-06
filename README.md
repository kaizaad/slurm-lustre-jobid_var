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
