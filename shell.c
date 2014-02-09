#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parse.h"   /*include declarations for parse-related structs*/
#include "hist_func.h" 

enum
BUILTIN_COMMANDS { NO_SUCH_BUILTIN=0, EXIT,JOBS,CD};
 
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
    asprintf(&dummy,"%s@%s$%s$",user,host,cwd);
    prompt = strdup(dummy);
    free(dummy);
    return prompt;
}
 
int
isBuiltInCommand(char * cmd){
  
  if ( strncmp(cmd, "exit", strlen("exit")) == 0){
    return EXIT;
  }

  if( strncmp(cmd, "cd", strlen("cd")) == 0){	
    return CD;
  }

  return NO_SUCH_BUILTIN;
}


int 
main (int argc, char **argv)
{
  const char* path;
  char* history[10];
  char* cmdLine;
  char* temp;
  char  dummy;
  char cmd;
  int hist_entry;
  int  hist_exec;
  int x;
  parseInfo *info; /*info stores all the information returned by parser.*/
  struct commandType *com; /*com stores command name and Arg list for one command.*/

#ifdef UNIX
  
    fprintf(stdout, "This is the UNIX version\n");
#endif

#ifdef WINDOWS
    fprintf(stdout, "This is the WINDOWS version\n");
#endif

  while(1){

#ifdef UNIX
/*if(!hist_exec){*/
    cmdLine = readline(buildPrompt());
    if (cmdLine == NULL) {
      fprintf(stderr, "Unable to read command\n");
      continue;
    }
/*}
else{
    hist_exec = 0;
}*/
#endif
    
    save_command(cmdLine,history);

    /*insert your code about history and !x !-x here*/
   /* if(sscanf(cmdLine,"!%d",hist_entry) != 0){
	    cmdLine = history[hist_entry];
	    hist_exec = 1;
	    continue;
    }
    if(cmdLine == "history"){
	display_history(history);
    }*/
    
    
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
      exit(1);
    }
  
    if (isBuiltInCommand(com->command) == CD){
      path = com->VarList[1];
      chdir(path);
    }


    /*insert your code here.*/

    free_info(info);
    free(cmdLine);
  }/* while(1) */
}
