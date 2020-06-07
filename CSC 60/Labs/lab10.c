/* Author(s): Austin Smothers.
 *
 * This is lab9.c the csc60mshell
 * This program serves as a skeleton for doing labs 9, 10, 11.
 * Student is required to use this program to build a mini shell
 * using the specification as documented in direction.
 * Date: Fall 2018
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAXLINE 80
#define MAXARGS 20
#define MAX_PATH_LENGTH 50
#define TRUE 1

/* function prototypes */
int parseline(char *cmdline, char **argv);

//The two function prototypes below will be needed in lab10. 
//Leave them here to be used later.
void process_input(int argc, char **argv);
void handle_redir(int count, char *argv[]);

/* ----------------------------------------------------------------- */
/*                  The main program starts here                     */
/* ----------------------------------------------------------------- */
int main(void)
{
    char cmdline[MAXLINE];
    char *argv[MAXARGS];
    int argc;
    int status;
    pid_t pid;

    /* Loop forever to wait and process commands */
    while (TRUE) {
	/* Print your shell name: csc60mshell (m for mini shell) */
	printf("csc60mshell> ");

	/* Read the command line */
	fgets(cmdline, MAXLINE, stdin);

	/* Call parseline to build argc/argv */
	argc = parseline(cmdline, argv);

	printf("Argc = %i\n", argc);
	int i=0;
	for(i = 0; i < argc; i++){
		printf("Argv %i = %s \n", i, argv[i]);
	}

	/* If user hits enter key without a command, continue to loop */
	/* again at the beginning */
	/*  Hint: if argc is zero, no command declared */
	/*  Hint: look up for the keyword "continue" in C */
	if(argc == 0){
		continue;
	}

	/* Handle build-in command: exit, pwd, or cd  */
	/* Put the rest of your code here */
	if(strcmp(argv[0], "exit")==0){
		exit(EXIT_SUCCESS);
	}
	else if(strcmp(argv[0], "pwd")==0){
		char arr[MAX_PATH_LENGTH];
		if(getcwd(arr, MAX_PATH_LENGTH)!=NULL){
			printf("%s\n", arr);
		}
		else{
			perror("\nError executing command.\n");
		}
		continue;
	}
	else if(strcmp(argv[0], "cd")==0){
		char *dir;
		if(argc == 1){
			dir = getenv("HOME");
		}
		else{
			dir = argv[1];
		}
		if(chdir(dir)!=0){
			perror("\nError changing directory.\n");
		}
		continue;
	}
	
	/* Else, fork off a process */
	else {
	    pid = fork();
            switch(pid)
    	    {
	    	case -1:
		    perror("Shell Program fork error");
	            exit(1);
	   	case 0:
		    /* I am child process. I will execute the command, */
		    /* and call: execvp */
		    process_input(argc, argv);
		    break;
	   	default:
		    /* I am parent process */
		    if (wait(&status) == -1)
		    	perror("Parent Process error");
		    else
		   	printf("Child returned status: %d\n",status);
		    break;
	    } 	/* end of the switch */
	}	/* end of the if-else-if */
    }		/* end of the while */
} 		/* end of main */

/* ----------------------------------------------------------------- */
/*                  parseline                                        */
/* ----------------------------------------------------------------- */
/* parse input line into argc/argv format */

int parseline(char *cmdline, char **argv)
{
    int count = 0;
    char *separator = " \n\t"; /* Includes space, Enter, Tab */
 
    /* strtok searches for the characters listed in separator */
    argv[count] = strtok(cmdline, separator);

    while ((argv[count] != NULL) && (count+1 < MAXARGS)) 
    	argv[++count] = strtok((char *) 0, separator);
     		
    return count;  //This becomes "argc" back in main.
}
/* ----------------------------------------------------------------- */
/*                  process_input                                    */
/* ----------------------------------------------------------------- */
void process_input(int argc, char **argv) {

    /* Step1: Call handle_redir to deal with operators.  */
    /* <, >, or both                                     */
    handle_redir(argc, argv);
    int return_value = execvp(argv[0], argv);


    /* Step 2: perform system call execvp to execute command        */
    /*Hint: Please be sure to review execvp.c sample program        */
    if (return_value == -1){
        fprintf(stderr, "Error on the exec call.\n");
        _exit(EXIT_FAILURE);
    }                                                            
 
}
/* ----------------------------------------------------------------- */
void handle_redir(int count, char *argv[]){
    int out_redir = 0;
    int in_redir = 0;
    int fd = 0;
    int i = 0;

    for(i=0; i < count; i++){
	if(strcmp(argv[i], ">") == 0){
	    if(out_redir != 0){
		perror("Cannot output to more than one file.");
		_exit(EXIT_FAILURE);
	    }
	    else if(i == 0){
		perror("No command entered.");
		_exit(EXIT_FAILURE);
	    }
	    out_redir = i;
	}
	else if(strcmp(argv[i], "<") == 0){
	    if(in_redir != 0){
		perror("Cannot input from more than one file.");
		_exit(EXIT_FAILURE);
	    }
	    else if(i == 0){
		perror("No command entered.");
		_exit(EXIT_FAILURE);
	    }
	    in_redir = i;
	}
    }
    if(out_redir != 0){
	if(!argv[out_redir+1]){
	    perror("There is no out file.");
	    _exit(EXIT_FAILURE);
	}
	fd = open(argv[out_redir+1], 
		  O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fd == -1){
	    perror("Error opening out file.");
	    _exit(EXIT_FAILURE);
	}
	dup2(fd, 1);
	if(close(fd) == -1){
	    perror("Error closing out file.");
	    _exit(EXIT_FAILURE);
	}
	argv[out_redir] = NULL;
    }

    if(in_redir !=0){
	if(!argv[in_redir+1]){
	    perror("There is no in file.");
	    _exit(EXIT_FAILURE);
	}
	fd = open(argv[in_redir+1], O_RDONLY);
	if(fd == -1){
	    perror("Error opening in file.");
	    _exit(EXIT_FAILURE);
	}
	dup2(fd, 0);
	if(close(fd) == -1){
	    perror("Error closing in file.");
	    _exit(EXIT_FAILURE);
	}
	argv[in_redir] = NULL;
    }
}

/* ----------------------------------------------------------------- */
