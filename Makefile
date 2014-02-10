LIB=-lreadline -lcurses
CFLAGS=-DUNIX -ansi -pedantic-errors
DEBUG=-g
#DEBUG=

all: shell

shell:	shell.c parse.c parse.h hist_func.c hist_func.h shared.h jobs.h jobs.c
	gcc $(CFLAGS) $(DEBUG) shell.c parse.c hist_func.c jobs.c -o a.out $(LIB)
clean:
	rm -f shell *~

