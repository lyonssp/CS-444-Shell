#include <stdio.h>
#include <stdlib.h>
#include "shared.h"

void save_job(char* job, char* array[]);
int jobs_empty(char* jobs[]);
void display_jobs(char* jobs[]);
void kill_job(int pid, char* job, char* jobs[]);
