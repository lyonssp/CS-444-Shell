CS-444-Shell
============

Implementation of a shell environment in C


----------------CODE STATUS------------------------
KNOWN ISSUES/BUGS:
    -Jobs can be sent to background, but doing so messes up the shell.
    -Sending jobs to background messes with STDIN it seems, making it near impossible to test the "jobs" command.
    -"Kill" is not fully functional.
    -Entering the "!-x" command corrupts history, but works otherwise.

FEATURES:
    Prompt:
	-Prints working directory appended to a string of format "USERNAME@HOSTNAME$$$CURRENT_WORKING_DIRECTORY$$$"
    I/O Redirection:
	-Fully functional
	-Can redirect input and output using "<" and ">" operators
    Place Commands in Background:
	-This feature is bugged, but according to the output of the shell skeleton code, the process is placed in the background.
    Check Background Jobs by Job:
	-There is an implementation, but it cannot be tested due to the bugs with the previous feature.
    History List:
	-Fully Functional
	-10 most recent commands are stored and can be viewed with the "history" command
    Repeat commands with "!x" and "!-x":
	-Only one bug:  "!-x" corrupts some history entries
    CD, EXIT, and KILL:
	-cd and exit work fine, but kill is buggy.
	-Exit will check to make sure there are no background jobs before exiting the shell
    
