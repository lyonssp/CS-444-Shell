#include < stdio.h>
#include <stdlib.h>
#include "shared.h"

void save_job(char* job, char* array[]);
int isBackgroundJob(char* job);
int jobs_empty(char* jobs[]);
