/* 
Austin Smothers
CSC 139
smothera-asgmt2.c
A program to simulate different scheduling algorithms and ouput
their turnaround time, response time, and waiting time
*/


/*======================================================
         IMPORTS AND GLOBAL VARIABLE DECLARATION
======================================================*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int system_time = 0;
float system_used_time = 0;
int size = 0;
const char* FCFS = "FCFS";
const char* RR = "RR";
const char* SRTF = "SRTF";
const char* RUNNING = "running";
const char* FINISHED = "finished.......";
size_t LB_SIZE = 99;

FILE *file;

struct process {
    int pid;			    // a process' unique PID
    int arrival_time;		// time when the task arrives in msec
    int burst_time;		    // CPU time requested by task in msec
    int wait_time;		    // time spent by a task waiting in msec
    int response_time;	    // time spent until a task runs once in msec
    int turnaround_time;	// time spent until a task finishes in msec
    int is_done;            // boolean int to tell if task is done or not
    struct process* next;   // Pointer to the next element
};
struct metrics {
    float total_wait_time;	        // total time spent by tasks waiting in msec
    float total_response_time;	    // total time spent until tasks run once in msec
    float total_turnaround_time;	// total time until all tasks finish in msec
    float average_wait_time;        // average time spent by tasks waiting in msec
    float average_response_time;    // average time till tasks start in msec
    float average_turnaround_time;  // average time till tasks complete in msec
    float average_cpu_usage;        // average % cpu used while running the scheduler
};


/*======================================================
          AUXILIARY FUNCTION DECLARATION BLOCK
======================================================*/

// The below functions all contain setup necessary to run this
// program
int isValidFile(const char* fname);
int getNumLines(const char* fname);
void fillProcArr(const char* fname, struct process proc_arr[]);
// The below functions all define the behavior of the different
// scheduler algorithms
void doFCFS(struct process proc_arr[]);
void doRR(struct process proc_arr[], int interval);
void doSRTF(struct process proc_arr[]);
// The below functions are part of a radix sort used by
// all three schedulers
void sortPIDAndArrivalTime(struct process proc_arr[]);
void sortPID(struct process proc_arr[]);
void sortArrivalTime(struct process proc_arr[]);
void sortIsDone(struct process proc_arr[]);
// The below function is part of a radix sort uniquely used
// by SRTF
void sortBurstTime(struct process proc_arr[]);
// The below functions are universally used by all three
// scheduler algorithms
void linkProcesses(struct process proc_arr[]);
int handleTimeAdjustments(struct process proc_arr[], int len);
void relink(struct process proc_arr[]);
int lRotate(struct process proc_arr[], int len);
int getLen(struct process proc_arr[]);
// Round Robin Stuff
void unlinkProcesses(struct process proc_arr[]);
void addToQueue(struct process proc_arr[]);
int handlePreemptCounter(struct process proc_arr[], struct process head, int last_idx, int preempt_counter);
// The below functions are used for output
void printProcArr(struct process proc_arr[]);
void printProcArr2(struct process proc_arr[]);
void printTimeUpdate(struct process proc_arr[], const char* status);
void handleMetrics(struct process proc_arr[]);


/*======================================================
                   MAIN FUNCTION BLOCK
======================================================*/

int main(int argc, char *argv[]) { 

    //local variable declaration
    char reportType[16];
    char fname[30];
    char schedType[5];
    int interval = 0;
 
    /* Determine report type */   
    if (argc > 2) { 
    	strcpy(fname, argv[1]);
	    strcpy(schedType, argv[2]);
	    if (!isValidFile(fname)) { 
	        fprintf(stderr, "Provided file does not exist\n"); 
	        return(1); 
	    }
	    if (strcmp(schedType, FCFS) == 0)
	        strcpy(reportType, FCFS); 
	    else if (strcmp(schedType, RR) == 0) { 
	        strcpy(reportType, RR); 
	        if (argv[3])
	    	    interval = atoi(argv[3]);
	        else{
		        fprintf(stderr, "usage: argument must be of format [RR] "
				        "[time_quantum]\n");
		        return(1);
	        }
	    }
	    else if (strcmp(schedType, SRTF) == 0)
	        strcpy(reportType, SRTF);
        else {
            fprintf(stderr, "usage: argument must be of format [FCFS|RR|SRTF] "
                "[time_quantum]\n");
            return(1);
        }
    }
    else {
	    fprintf(stderr, "usage: proj2 [inputFile] [FCFS|RR|SRJF] [timeQuantum]\n");
	    return(1);
    }
    
    // This is safe because there is no way for c1 to not have a user defined value
    size = getNumLines(fname);
    struct process proc_arr[size];
    fillProcArr(fname, proc_arr);
    /* 
    All three schedulers want their input sorted by PID then Arrival time
    *priority of sorting listed from least to greatest* 
    */
    sortPIDAndArrivalTime(proc_arr);
    // Don't bother linking for RR. It'll link its own stuff
    if (strcmp(reportType, RR) != 0) {
        linkProcesses(proc_arr);
    }
    // Finish initialization
    printf("Scheduling algorithm:  %s\n", reportType);
    printf("Total %d tasks are read from \"%s\". Press 'enter' to start...\n", 
    	   size, fname);
    getchar();

    //printProcArr(proc_arr, numLines);

    /* Code to perform the relevant scheduling algorithm */
    if(strcmp(reportType, FCFS) == 0)
    	doFCFS(proc_arr);
    if(strcmp(reportType, RR) == 0)
    	doRR(proc_arr, interval);
    if(strcmp(reportType, SRTF) == 0)
	    doSRTF(proc_arr);
  
    printf("<system time %4d> All processes finish....................\n", system_time);
    handleMetrics(proc_arr);

    return 0;
}


/*========================================================
           AUXILIARY FUNCTION DEFINITION BLOCK
========================================================*/

/*--------------------------------------------------------
                  SETUP FUNCTIONS
--------------------------------------------------------*/

// A function to verify that the user provided file is real and can be read
int isValidFile(const char* fname){
    if(access(fname, R_OK) != -1){
    	return 1;
    }
    else{
    	return 0;
    }
}

// A function to determine the number of lines in a file
int getNumLines(const char* fname){
    char line[LB_SIZE];
    int count = 0;
    file = fopen(fname, "r");
    //read file
    while (fgets(line, LB_SIZE+1, file) != NULL) {
    	count++;
    }
    fclose(file);
    return count;
}

// A function to fill our array of processes
void fillProcArr(const char* fname, struct process proc_arr[]){
    char line[LB_SIZE];
    int i = 0;
    file = fopen(fname, "r");
    //read file
    while (fgets(line, LB_SIZE+1, file) != NULL) {
    	sscanf(line, "%d %d %d", &proc_arr[i].pid, 
	       &proc_arr[i].arrival_time, &proc_arr[i].burst_time);
	    proc_arr[i].wait_time = 0; 
	    proc_arr[i].response_time = 0;
	    proc_arr[i].turnaround_time = 0;
        proc_arr[i].is_done = 0;
	    i++;
    }
    fclose(file);
}

/*--------------------------------------------------------
                SCHEDULER ALGORITHMS
--------------------------------------------------------*/

// A function to perform FCFS Scheduling
void doFCFS(struct process proc_arr[]) {
    int needs_to_run = 1;
    while (needs_to_run) {
        /*printf("Proc Arr:\n");
        printProcArr2(proc_arr);*/
        if (needs_to_run && proc_arr[0].is_done == 0)
            handleTimeAdjustments(proc_arr, size);
        // If our lead process is already done, rotate it out of 
        // the way until we have an unfinished process as leader
        // If we have no unfinished process, we're done
        if (proc_arr[0].is_done)
            needs_to_run = lRotate(proc_arr, size);
    }
}

// A function to perform RR Scheduling
void doRR(struct process proc_arr[], int interval) {
    int preempt_counter = 0, len = 0;
    int needs_to_run = 1;
    int should_increment_preempt_counter = 0;
    struct process head;
    
    // Unlink and then close our circularly linked list used for RR
    unlinkProcesses(proc_arr);
    proc_arr[0].next = &proc_arr[0];

    while (needs_to_run) {
        /*printf("Proc Arr:\n");
        printProcArr2(proc_arr);*/
        // Check for and add any new processes to our RR queue
        addToQueue(proc_arr);
        len = getLen(proc_arr);
        // simulate pre-emption by right rotating at each interval
        if ((system_time > 0) && (preempt_counter > 0) &&
            (preempt_counter % interval == 0) && (proc_arr[0].burst_time > 0)) {
            needs_to_run = lRotate(proc_arr, len);
            preempt_counter = 0;
        }
        head = proc_arr[0];
        if (needs_to_run)
            should_increment_preempt_counter = handleTimeAdjustments(proc_arr, len);
        // Only run if the head process isn't done
        if (proc_arr[0].is_done)
            needs_to_run = lRotate(proc_arr, len);
        preempt_counter = handlePreemptCounter(proc_arr, head, len - 1, preempt_counter);
        if (should_increment_preempt_counter)
            preempt_counter++;
    }
}

// A function to perform SRTF Scheduling
void doSRTF(struct process proc_arr[]) {
    int needs_to_run = 1;
    int len = 0;
    while (needs_to_run) {
        // simulate pre-emption by re-sorting at each interval
        sortBurstTime(proc_arr);
        sortIsDone(proc_arr);
        relink(proc_arr);
        len = getLen(proc_arr);
        // If our lead process is already done, rotate it out of 
        // the way until we have an unfinished process as leader
        if (proc_arr[0].is_done || proc_arr[0].arrival_time > system_time)
            needs_to_run = lRotate(proc_arr, len);
        if (needs_to_run && proc_arr[0].is_done == 0)
            handleTimeAdjustments(proc_arr, len);
    }
}

/*--------------------------------------------------------
                    UNIVERSAL SORTING
--------------------------------------------------------*/

// A function to sort our input based on PID and Arrival time
void sortPIDAndArrivalTime(struct process proc_arr[]) {
    /* 
    Always sort by the lowest priority item first.
    In this case, lowest priority is always PID because
    we cannot run a process which has not yet arrived 
    */
    sortPID(proc_arr);
    sortArrivalTime(proc_arr);
}

// Count sort our processes based on their PID
void sortPID(struct process proc_arr[]) {
    struct process aux[size];
    int largest = 0, i = 0;

    for (i = 0; i < size; i++) {
        aux[i] = proc_arr[i];
        // Get the largest arrival time so we can count sort 
        if (proc_arr[i].pid > largest) {
            largest = proc_arr[i].pid;
        }
    }
    // Create a count array to store count of individual
    // processes by their PID's
    int count[largest + 1];

    for (i = 0; i <= largest; i++) {
        count[i] = 0;
    }
    for (i = 0; i < size; i++) {
        count[proc_arr[i].pid]++;
    }
    // Change count[i] so that count[i] now contains actual 
    // position of this process in output array 
    for (i = 1; i <= largest; i++)
        count[i] += count[i - 1];
    // Build the aux output array 
    for (i = size-1; i >= 0; i--) {
        aux[count[proc_arr[i].pid] - 1] = proc_arr[i];
        count[proc_arr[i].pid]--;
    }
    // Copy aux to proc_arr, so that proc_arr now 
    // contains processes sorted by PID
    for (i = 0; i < size; i++)
        proc_arr[i] = aux[i];
}

// Count sort our processes based on their arrival time
void sortArrivalTime(struct process proc_arr[]){
    struct process aux[size];
    int largest = 0, i = 0;

    for (i = 0; i < size; i++) {
        aux[i] = proc_arr[i];
        // Get the largest arrival time so we can count sort 
        if (proc_arr[i].arrival_time > largest) {
            largest = proc_arr[i].arrival_time;
        }
    }
    // Create a count array to store count of individual
    // processes by their arrival times
    int count[largest + 1];

    for (i = 0; i <= largest; i++) {
        count[i] = 0;
    }
    for (i = 0; i < size; i++) {
        count[proc_arr[i].arrival_time]++;
    }
    // Change count[i] so that count[i] now contains actual 
    // position of this process in output array 
    for (i = 1; i <= largest; i++)
        count[i] += count[i - 1];
    // Build the aux output array 
    for (i = size - 1; i >= 0; i--) {
        aux[count[proc_arr[i].arrival_time] - 1] = proc_arr[i];
        count[proc_arr[i].arrival_time]--;
    }
    // Copy aux to proc_arr, so that proc_arr now 
    // contains processes sorted by arrival time
    for (i = 0; i < size; i++)
        proc_arr[i] = aux[i];
}

// Count sort our processes based on their is_done status
void sortIsDone(struct process proc_arr[]) {
    struct process aux[size];

    // We don't need to calculate largest since we're sorting binary data, just set to 1
    int largest = 1, i = 0;

    for (i = 0; i < size; i++) {
        aux[i] = proc_arr[i];
    }
    // Create a count array to store count of individual
    // processes by their arrival times
    int count[largest + 1];

    for (i = 0; i <= largest; i++) {
        count[i] = 0;
    }
    for (i = 0; i < size; i++) {
        count[proc_arr[i].is_done]++;
    }
    // Change count[i] so that count[i] now contains actual 
    // position of this process in output array 
    for (i = 1; i <= largest; i++)
        count[i] += count[i - 1];
    // Build the aux output array 
    for (i = size - 1; i >= 0; i--) {
        aux[count[proc_arr[i].is_done] - 1] = proc_arr[i];
        count[proc_arr[i].is_done]--;
    }
    // Copy aux to proc_arr, so that proc_arr now 
    // contains processes sorted by arrival time
    for (i = 0; i < size; i++) {
        proc_arr[i] = aux[i];
        if (proc_arr[i].is_done)
            proc_arr[i].next = NULL;
    }

    for (i = 0; i < size - 1; i++) {
        if (proc_arr[i].is_done == 0 && proc_arr[i].arrival_time <= system_time) {
            if (proc_arr[i + 1].next != NULL)
                proc_arr[i].next = &proc_arr[i + 1];
            else
                proc_arr[i].next = &proc_arr[0];
        }
    }

}

/*--------------------------------------------------------
                    SRTF SORTING
--------------------------------------------------------*/

void sortBurstTime(struct process proc_arr[]) {
    struct process aux[size];
    int largest = 0, i = 0;

    for (i = 0; i < size; i++) {
        aux[i] = proc_arr[i];
        // Get the largest arrival time so we can count sort 
        if (proc_arr[i].burst_time> largest) {
            largest = proc_arr[i].burst_time;
        }
    }
    // Create a count array to store count of individual
    // processes by their burst times
    int count[largest + 1];

    for (i = 0; i <= largest; i++) {
        count[i] = 0;
    }
    for (i = 0; i < size; i++) {
        count[proc_arr[i].burst_time]++;
    }
    // Change count[i] so that count[i] now contains actual 
    // position of this process in output array 
    for (i = 1; i <= largest; i++)
        count[i] += count[i - 1];
    // Build the aux output array 
    for (i = size - 1; i >= 0; i--) {
        aux[count[proc_arr[i].burst_time] - 1] = proc_arr[i];
        count[proc_arr[i].burst_time]--;
    }
    // Copy aux to proc_arr, so that proc_arr now 
    // contains processes sorted by burst time
    for (i = 0; i < size; i++)
        proc_arr[i] = aux[i];
}

/*--------------------------------------------------------
                   UNIVERSAL MAINTENANCE
--------------------------------------------------------*/

// Function to establish our array of processes as a circularly linked list
void linkProcesses(struct process proc_arr[]) {
    for (int i = 0; i < size; i++) {
        if (i < size - 1)
            proc_arr[i].next = &proc_arr[i + 1];
        else
            proc_arr[i].next = &proc_arr[0];
    }
}

// Function to handle adjustment of time variables per system_time tick
int handleTimeAdjustments(struct process proc_arr[], int len) {
    int can_increment_preempt = 0;
    for(int i = 0; i < len; i++){
	    if(proc_arr[i].arrival_time <= system_time && proc_arr[i].is_done == 0){
            if (proc_arr[i].burst_time == 0) {
                proc_arr[i].is_done = 1;
                printTimeUpdate(proc_arr, FINISHED);
                sortIsDone(proc_arr);
            }
            if(i > 0)
	    	    proc_arr[i].wait_time++;
            if (proc_arr[i].burst_time > 0 && proc_arr[i].arrival_time <= system_time){
                if (i == 0)
	    	        proc_arr[i].burst_time--;
	    	    proc_arr[i].turnaround_time++;
	    	    if(proc_arr[i].wait_time == proc_arr[i].turnaround_time)
		            proc_arr[i].response_time++;
	        }
	    }
    }
    if (proc_arr[0].is_done == 0) {
        if (proc_arr[0].arrival_time <= system_time) {
            printTimeUpdate(proc_arr, RUNNING);
            system_used_time++;
            can_increment_preempt = 1;
        }
        system_time++;
    }
    return can_increment_preempt;
}

// Function to relink processes that aren't done.
// Don't use for RR, it has special linking requirements
void relink(struct process proc_arr[]) {
    for (int i = 0; i < size - 1; i++) {
        if (proc_arr[i].is_done == 0) {
            if (proc_arr[i + 1].next != NULL)
                proc_arr[i].next = &proc_arr[i + 1];
            else
                proc_arr[i].next = &proc_arr[0];
        }
    }
    if (proc_arr[size - 1].next != NULL)
        proc_arr[size - 1].next = &proc_arr[0];
}

// Function to left rotate the array of processes
int lRotate(struct process proc_arr[], int len) {
    int i = 0, rotations = 0;
    struct process temp;
    do {
        temp = proc_arr[0];
        /*printf("Temp:\n");
        printf("%d %d %d %d %d %d %d %d\n", temp.pid, temp.arrival_time,
            temp.burst_time, temp.wait_time,
            temp.response_time, temp.turnaround_time,
            temp.is_done, temp.next->pid);*/
        // find the element that closes our circular linked list
        for (i = 0; i < len; i++) {
            if (proc_arr[i].next == &proc_arr[0])
                proc_arr[i].next = &temp;
        }
        /*printf("Proc Arr After Making Mods to closer (Exec):\n");
        printProcArr(proc_arr);*/
        // update all elements that don't point back to the head
        for (i = 0; i < len - 1; i++) {
            if (proc_arr[i].next != NULL) {
                proc_arr[i] = *proc_arr[i].next;
                proc_arr[i].next = &proc_arr[i + 1];
            }
        }
        /*
        don't printProcArr here. there are unhandled circular sub-lists.
        */
        // find the element that pointed to the head and update it
        for (i = 0; i < len; i++) {
            if (proc_arr[i].next == &temp) {
                proc_arr[i] = temp;
                proc_arr[i].next = &proc_arr[0];
            }
        }
        /*printf("Proc Arr After P (Exec):\n");
        printProcArr(proc_arr);*/
        // don't rotate through the full list more than once
        if (rotations == len) {
            /*printf("Proc Arr After Rotating (Done):\n");
            printProcArr(proc_arr);*/
            // if some process still needs to run, don't return 0 (done)
            for (i = 0; i < size; i++) {
                if (proc_arr[i].is_done == 0)
                    return 1;
            }
            return 0;
        }
        rotations++;
    } while (proc_arr[0].is_done || (proc_arr[0].arrival_time > system_time)); 
    /*printf("Proc Arr After Rotating (Exec):\n");
    printProcArr(proc_arr);*/
    return 1;
}

// Function to get the remaining length of our process array
int getLen(struct process proc_arr[]) {
    struct process walker = proc_arr[0];
    int len = 0;
    // Walk to calculate our length
    walker = proc_arr[0];
    while (walker.next != NULL && walker.next->pid != proc_arr[0].pid) {
        walker = *walker.next;
        len++;
    }
    // Add one more time to change len from the int of the last index
    // to the actual length of the array
    len++;

    return len;
}

/*--------------------------------------------------------
                   ROUND ROBIN STUFF
--------------------------------------------------------*/

// function to unlink our nodes
void unlinkProcesses(struct process proc_arr[]) {
    for (int i = 0; i < size; i++) {
        proc_arr[i].next = NULL;
    }
}

// function to add a new process to the RR queue
void addToQueue(struct process proc_arr[]) {
    struct process walker = proc_arr[0];
    int i = 0, len = 0;

    // walk down to the last node in our circularly linked list
    while (walker.next != NULL && walker.next->pid != proc_arr[0].pid) {
        walker = *walker.next;
        i++;
    }

    // i gets walked to the necessary value by walker above
    for (i; i < size; i++) {
        if (proc_arr[i].arrival_time == system_time) {
            proc_arr[i - 1].next = &proc_arr[i];
            proc_arr[i].next = &proc_arr[0];
        }
    }
}

// Handles the resetting of the preemption counter
int handlePreemptCounter(struct process proc_arr[], struct process head, int last_idx, int preempt_counter) {
    // reset the pre-emption counter as we just removed a completed process
    if ((proc_arr[last_idx].is_done && last_idx > 0) || head.pid != proc_arr[0].pid)
        preempt_counter = 0;
    return preempt_counter;
}

/*--------------------------------------------------------
                   WE PRINT STUFF HERE
--------------------------------------------------------*/

//test function to print the proc array. Used strictly for debugging
void printProcArr(struct process proc_arr[]) {
    struct process walker = proc_arr[0];
    while(&walker != NULL && walker.next != NULL && walker.next->pid != proc_arr[0].pid) {
        printf("%d %d %d %d %d %d %d %d\n", walker.pid, walker.arrival_time,
            walker.burst_time, walker.wait_time,
            walker.response_time, walker.turnaround_time,
            walker.is_done, walker.next->pid);
        walker = *walker.next;
    }
    if (&walker != NULL && walker.next != NULL) {
        printf("%d %d %d %d %d %d %d %d\n", walker.pid, walker.arrival_time,
            walker.burst_time, walker.wait_time,
            walker.response_time, walker.turnaround_time,
            walker.is_done, walker.next->pid);
    }
    printf("Sys time %d\nUsed Time %f\n", system_time, system_used_time);
}

//test function to print the proc array. Used strictly for debugging
void printProcArr2(struct process proc_arr[]) {
    for(int i = 0; i < size; i++){
        if (proc_arr[i].next != NULL) {
            printf("%d %d %d %d %d %d %d %d\n", proc_arr[i].pid, proc_arr[i].arrival_time,
                proc_arr[i].burst_time, proc_arr[i].wait_time,
                proc_arr[i].response_time, proc_arr[i].turnaround_time,
                proc_arr[i].is_done, proc_arr[i].next->pid);
        }
        else {
            printf("%d %d %d %d %d %d %d NULL\n", proc_arr[i].pid, proc_arr[i].arrival_time,
                proc_arr[i].burst_time, proc_arr[i].wait_time,
                proc_arr[i].response_time, proc_arr[i].turnaround_time,
                proc_arr[i].is_done);
        }
    }
    printf("Sys time %d\nUsed Time %f\n", system_time, system_used_time);
}

// Function to handle basic printing of time update values
void printTimeUpdate(struct process proc_arr[], const char* status){
    printf("<system time %4d> process %4d is %s\n", system_time, proc_arr[0].pid, status);
}

//function to calculate and print the metrics for an algorithm
void handleMetrics(struct process proc_arr[]){
    struct metrics m = {0, 0, 0, 0, 0, 0, 0};
    for(int i = 0; i < size; i++){
	    m.total_wait_time += proc_arr[i].wait_time;
	    m.total_response_time += proc_arr[i].response_time;
	    m.total_turnaround_time += proc_arr[i].turnaround_time;
    }
    m.average_wait_time = (float)(m.total_wait_time/size);
    m.average_response_time = (float)(m.total_response_time/size);
    m.average_turnaround_time = (float)(m.total_turnaround_time/size);
    m.average_cpu_usage = (float)((system_used_time*100)/system_time);
    printf("=========================================================\n");
    printf("Average cpu usage\t: %.2f %%\n", m.average_cpu_usage);
    printf("Average waiting time\t: %.2f\n", m.average_wait_time);
    printf("Average response time\t: %.2f\n", m.average_response_time);
    printf("Average turnaround time\t: %.2f\n", m.average_turnaround_time);
    printf("=========================================================\n");
}
