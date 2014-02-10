#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"
#include <signal.h>

/*Store background process in jobs array*/
void save_job(char* job, char* array[]){
    int i;

    if(bg_jobs_count < jobs_max_size){
        array[bg_jobs_count] = (char*)malloc((strlen(job)+1)*sizeof(char));
        strcpy(array[bg_jobs_count],job);
    }
    else{
        array[jobs_max_size-1] = (char*)malloc((strlen(job)+1)*sizeof(char));
        for(i=0; i<jobs_max_size-1 ; i++){
            free(array[i]);
            array[i] = (char*)malloc( (strlen(array[i+1])+1)*sizeof(char));
            strcpy(array[i],array[i+1]);
        }
        free(array[jobs_max_size-1]);
        array[jobs_max_size-1] = (char*)malloc((strlen(job)+1)*sizeof(char));
        strcpy(array[jobs_max_size-1],job);
    }

    bg_jobs_count++;
}

void display_jobs(char* jobs[]){
    int i;
    if(bg_jobs_count<jobs_max_size){
        for(i=0; i<bg_jobs_count; i++){
            printf("[%d] %s\n",i+1,jobs[i]);
        }
    }
    else{
        for(i=0;i<jobs_max_size;i++){
            printf("[%d] %s\n",i+1,jobs[i]);
        }
    }
}

/*Return 0 or 1 to indicate whether there are any background jobs when function is called*/
int jobs_empty(char* jobs[]){
    int i = 0;

    /*Job char pointers will all be initialized to NULL and set to NULL when process terminates*/
    while(jobs[i]==NULL){
	i++;	
    }

    if(i == jobs_max_size)
	return 1;
    else
	return 0;
}

/*Return 0 or 1 to indicate successful functional call*/
int kill_job(int pid, char* job, char* jobs[]){
    int i = 0;
    
    /*Can return in the loop because pid is unique*/
    for(i = 0; i < jobs_max_size; i++){
	if(strcmp(jobs[i],job)==0){
	    free(jobs[i]);
	    jobs[i] = NULL;
	    kill(pid,SIGKILL);
	    return 1;
	}
    }
    
    if(i == jobs_max_size){
	return 0;
    }
}

