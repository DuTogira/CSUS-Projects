// Austin Smothers
// CSC 139
// smothera-asgmt1.c
// A program to obtain certain process info about a host computer
// dynamically based on arguments provided when running the program


/*------------------------------------------------------
         IMPORTS AND GLOBAL VARIABLE DECLARATION
------------------------------------------------------*/

#include <stdio.h> 
#include <sys/time.h>
#include <time.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <chrono>
using namespace std;

const string STANDARD = "STANDARD";
const string SHORT = "SHORT";
const string LONG = "LONG";
const int LB_SIZE = 99;

FILE *thisProcFile;
struct timeval now;


/*------------------------------------------------------
          AUXILIARY FUNCTION DECLARATION BLOCK
------------------------------------------------------*/

void exec(const char* cmd);
void doStandard();
void doShort();
void doLong();


/*------------------------------------------------------
                   MAIN FUNCTION BLOCK
------------------------------------------------------*/

int main(int argc, char *argv[]) { 

    //local variable declaration
    char repTypeName[16];
    char c1, c2;
    int interval = 1;
    int duration, iteration = 0;
 
    /* Determine report type */ 
    string reportType = STANDARD; 
    strcpy(repTypeName, "Standard"); 
    if (argc > 1) { 
    	sscanf(argv[1], "%c%c", &c1, &c2); 
	if (c1 != '-') { 
	    fprintf(stderr, "usage: observer [-s][-l int dur]\n"); 
	    return(1); 
	} 
	if (c2 == 's') { 
	    reportType = SHORT; 
	    strcpy(repTypeName, "Short"); 
	} 
	if (c2 == 'l') { 
	    reportType = LONG; 
	    strcpy(repTypeName, "Long"); 
	    if (argv[2] && argv[3]){
	    	interval = atoi(argv[2]); 
	    	duration = atoi(argv[3]);
	    }
	} 
    }

    bool isStandard = (reportType == STANDARD || reportType == SHORT || 
                       reportType == LONG);
    bool isShort = (reportType == SHORT || reportType == LONG);
    bool isLong = (reportType == LONG);
    
    /* Finish initialization */ 
    gettimeofday(&now, NULL); // Get the current time 
    printf("Status report type %s at %s\n", repTypeName, ctime(&(now.tv_sec)));

    /* Code to read the relevant /proc files */
    if(isStandard) {
    	doStandard();
    }
    if(isShort) {
    	doShort();
    }
    if(isLong) {
	doLong();

    	/* Sample for load averages */
    	while (iteration < duration) { 
    	    sleep(interval); 
	    printf("\nAverage Load Sample (1m | 5m | 10m | proc | pid) : ");
	    exec("cat /proc/loadavg"); 
	    iteration += interval; 
    	}
    }

    return 0;
}


/*--------------------------------------------------------
           AUXILIARY FUNCTION DEFINITION BLOCK
--------------------------------------------------------*/

// A function to execute console commands and capture their output
void exec(const char* cmd) {
    char output[LB_SIZE];
    thisProcFile = popen(cmd, "r");
    if(!thisProcFile) { 
    	printf("Failed to run the process.\n");
    }
    else {
    	while(fgets(output, LB_SIZE+1, thisProcFile) != NULL) {
	    printf("%s\n", output);
	}
    }
    pclose(thisProcFile);
}

// Execute all of the standard report commands
void doStandard() {
    /* Get the host filename and print it */
    printf("\nMachine hostname: \n");
    exec("cat /proc/sys/kernel/hostname");
    /* Get the CPU model name */
    printf("\nCPU Model Name: \n");
    exec("cat /proc/cpuinfo | grep model | grep name | sort -u");
    /* Get the Kernel Version */
    printf("\nKernel Version: \n");
    exec("cat /proc/version");
    /* Get the amount of time since the system was last booted */
    printf("\nBoot Data: \n");
    exec("uptime");
}

// Execute all of the short report commands
void doShort() {
    /* Get the time when the system was last booted */
    exec("who -b");
    /* Get the number of processes created since last boot */
    printf("\nProcesses created since last boot: \n");
    exec("cat /proc/stat | grep processes");
    /* Get the amount of time all CPUs have spent in different modes,
       as well as the number of disk read/write operations*/
    exec("iostat -m");
    /* Get the number of context switches the kernel has performed */
    printf("\nContext Switches: \n");
    exec("cat /proc/2/status | grep ctxt_switches");
}

//Execute all of the long report commands
void doLong() {
    /* Get memory statistics from the host */
    printf("\nMemory statistics:\n");
    exec("cat /proc/meminfo | grep Mem");
