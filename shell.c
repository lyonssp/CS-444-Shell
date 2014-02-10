#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parse.h"   /*include declarations for parse-related structs*/
#include "hist_func.h" 
#include "shared.h"
#include <signal.h>

/*Globals Declared in shared.h*/
int history_count = 0;
int hist_max_size = 10;
int bg_jobs_count = 0;
int jobs_max_size = 10;


enum
BUILTIN_COMMANDS { NO_SUCH_BUILTIN=0, EXIT,JOBS,CD,HISTORY,HELP,KILL};
 
char *
buildPrompt()
{
    char* user = getenv("USER");
    char* dummy;
    char* prompt;
    char host[1024];
    char cwd[1024];     
    
    getcwd(cwd,1024);
    gethostname(host,1024);
    asprintf(&dummy,"%s@%s$$$%s$$$ ",user,host,cwd);
    prompt = strdup(dummy);
    free(dummy);
    return prompt;
}
 
int
isBuiltInCommand(char * cmd){
  
  if ( strcmp(cmd, "exit") == 0){
    return EXIT;
  }

  if( strcmp(cmd, "cd") == 0){	
    return CD;
  }

  if( strcmp(cmd, "help") == 0){
    return HELP;
  }

  if( strcmp(cmd, "history") == 0){
    return HISTORY;
  }

  if( strcmp(cmd, "kill") == 0){
    return KILL;
  }
  
  if( strcmp(cmd, "jobs") == 0){
    return JOBS;
  }
 
  return NO_SUCH_BUILTIN;
}

int 
main (int argc, char **argv)
{
  int i;
  int id;
  pid_t pid;
  FILE* infile=NULL;
  FILE* outfile=NULL;
  const char* path;
  char* history[10];
  char* jobs[10];
  char* cmdLine;
  int hist_entry;
  int hist_exec = 0; /*will determine whether "!x" was the previous command.  Stalls readline for one "while" iteration*/
  int x;
  parseInfo *info; /*info stores all the information returned by parser.*/
  struct commandType *com; /*com stores command name and Arg list for one command.*/

/*Initialize jobs list so we can test its emptiness later*/
  for(i = 0; i < jobs_max_size; i++){
	jobs[i] = NULL;
  } 

#ifdef UNIX
  
    fprintf(stdout, "This is the UNIX version\n");
#endif

#ifdef WINDOWS
    fprintf(stdout, "This is the WINDOWS version\n");
#endif

  while(1){

#ifdef UNIX
if(!hist_exec){
    cmdLine = readline(buildPrompt());
    if (cmdLine == NULL) {
      fprintf(stderr, "Unable to read command\n");
      continue;
    }
}
else{
    hist_exec = 0;
}
#endif
    

    /*insert your code about history and !x !-x here*/
    if(sscanf(cmdLine,"!-%d",&hist_entry) != 0){
	/*ERROR CHECKING*/
	if(hist_max_size-hist_entry > history_count || hist_entry > 10 || hist_entry < 1){
	    fprintf(stderr,"!-%d does not specify a command in history\n",&hist_entry);
	    continue;
	}
	/*CHECK IF HISTORY BUFFER HAS BEEN FILLED*/
	if(history_count<hist_max_size){	
	    cmdLine = (char*)malloc((strlen(history[hist_entry])+1)*sizeof(char));
	    strcpy(cmdLine,history[history_count-hist_entry]);
	}
	else{
	    cmdLine = (char*)malloc((strlen(history[hist_entry])+1)*sizeof(char));
            strcpy(cmdLine,history[hist_max_size-hist_entry]);
	} 
	    hist_exec = 1;
	    continue;
    }
    else if(sscanf(cmdLine,"!%d",&hist_entry) != 0){
	/*ERROR CHECKING*/
	if((hist_entry-1 > history_count) || (hist_entry > 10) || (hist_entry < 1)){
	    fprintf(stderr,"!%d does not specify a command in history\n",&hist_entry);
	    continue;
	}	
	cmdLine = (char*)malloc((strlen(history[hist_entry])+1)*sizeof(char));
	strcpy(cmdLine,history[hist_entry-1]);
	hist_exec = 1;
	continue;
    }

    printf("SAVING COMMAND\n");
    save_command(cmdLine,history);
    
    /*calls the parser*/
    info = parse(cmdLine);
    if (info == NULL){
      free(cmdLine);
      continue;
    }
    /*prints the info struct*/
    print_info(info);

    /*com contains the info. of the command before the first "|"*/
    com=&info->CommArray[0];
    if ((com == NULL)  || (com->command == NULL)) {
      free_info(info);
      free(cmdLine);
      continue;
    }
    /*com->command tells the command name of com*/
    if (isBuiltInCommand(com->command) == EXIT){
      if(jobs_empty(jobs)){
	exit(1);
      }
      else{
	fprintf(stderr,"THERE ARE BACKGROUND PROCESSES.  KILL BEFORE EXITING\n");
	continue;
      }
    }
    else if (isBuiltInCommand(com->command) == CD){
      path = com->VarList[1];
      if(chdir(path) == -1){
	printf("ERROR IN DIRECTORY CHANGE\n");
	continue;
      }
    }
    else if (isBuiltInCommand(com->command) == HISTORY){
      display_history(history);
    }
    else if (isBuiltInCommand(com->command) == HELP){
      printf("cd <relative or absolute directory>\nkill %%jobnumber\nhistory\njobs\nexit\n");
    }
    else if (isBuiltInCommand(com->command) == KILL){
      id = (int) com->VarList[1];
      if((kill(id,SIGKILL))<0){
	fprintf(stderr,"ERROR SENDING KILL SIGNAL\n");
	continue;
      }
      else{
	printf("PROCESS %d SUCCESSFULLY KILLED\n",id);
      }
    }
    else if (isBuiltInCommand(com->command) == JOBS){
	display_jobs(jobs);
    }   

    /*Code for I/O Re-direct*/
    if(info->boolInfile || info->boolOutfile){
	
	if((pid = fork()) < 0){
	    fprintf(stderr,"I/O REDIRECT FORK FAILED\n");
	    continue;
	}
	else if(pid == 0){
	    if(info->boolInfile){
		infile = fopen(info->inFile, "r");
		dup2(fileno(infile),fileno(stdin));
		fclose(infile);
	    }
	    if(info->boolOutfile){		
		outfile = fopen(info->outFile, "w");
		dup2(fileno(outfile),fileno(stdout));
		fclose(outfile);
	    }
	    execvp(info->CommArray[0].command,info->CommArray[0].VarList);
	}
	else{
	    wait(0); /*wait for child process*/
	}
    }

    free_info(info);
    free(cmdLine);
    }
}

