#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "hist_func.h"
#include "shared.h"

/*insert "item" into "array"*/
void save_command(char* item, char *array[]){
    int i;    

    if(history_count < hist_max_size){
	array[history_count] = (char*)malloc((strlen(item)+1)*sizeof(char));
	strcpy(array[history_count],item);
    }
    else{
	array[hist_max_size-1] = (char*)malloc((strlen(item)+1)*sizeof(char));
	for(i=0; i<hist_max_size-1 ; i++){
	    free(array[i]);
	    array[i] = (char*)malloc( (strlen(array[i+1])+1)*sizeof(char)); 
	    strcpy(array[i],array[i+1]);
	}
	free(array[hist_max_size-1]);
	array[hist_max_size-1] = (char*)malloc((strlen(item)+1)*sizeof(char));
	strcpy(array[hist_max_size-1],item);
    }

    history_count++;
}

void display_history(char *history[]){
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
