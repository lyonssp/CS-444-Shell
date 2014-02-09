#include<stdio.h>
#include<stdlib.h>
#include "hist_func.h"

/*insert "item" into "array" of size "size" at index "index"*/
void insert(char* item, char* array[], size_t index, size_t size){
    int i;
    for(i = size-1; i > index; i--)
        array[i] = array[i-1];
    array[index] = item;
}

/*save most recent command "cmd" into history*/
void save_command(char* cmd, char* history[]){
    
    int i = 0;
    while(*history[i]!='\0'){
	i++;
    }
    insert(cmd,history,i,10);
}

void display_history(char* history[]){
    int i = 0;
    while(*history[i]!='\0'){
	printf("[%d] %s\n",i+1,*history[i]);
    }
}

