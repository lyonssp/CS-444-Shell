#include <stdio.h>
#include <stdlib.h>
#include "shared.h"

void save_job(char* job, char* array[]){
    int i;

    if(bg_job_count < jobs_max_size){
        array[bg_job_count] = (char*)malloc((strlen(job)+1)*sizeof(char));
        strcpy(array[bg_job_count],job);
    }
    else{
        array[jobs_max_size-1] = (char*)malloc((strlen(job)+1)*sizeof(char));
        for(i=0; i<jobs_max_size-1 ; i++){
            free(array[i]);
            array[i] = (char*)malloc( (strlen(array[i+1])+1)*sizeof(char));
            strcpy(array[i],array[i+1]);
        }
        free(jobs_max_size-1);
        array[jobs_max_size-1] = (char*)malloc((strlen(job)+1)*sizeof(char));
        strcpy(array[jobs_max_size-1],job);
    }

    bg_jobs_count++;
}

void display_jobs(char* jobs[]){
    int i;
    if(history_count<hist_max_size){
        for(i=0; i<history_count; i++){
            printf("[%d] %s\n",i+1,history[i]);
        }
    }
    else{
        for(i=0;i<hist_max_size;i++){
            printf("[%d] %s\n",i+1,history[i]);
        }
    }
}

