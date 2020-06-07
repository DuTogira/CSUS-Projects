/*
Austin Smothers
CSC 139
smothera-asgmt4.c
A program to perform page replacement using a specified algorithm.
*/


/*======================================================
         IMPORTS AND GLOBAL VARIABLE DECLARATION
======================================================*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char* FIFO = "FIFO";
const char* OPT = "OPT";
const char* LRU = "LRU";
size_t LB_SIZE = 99;

int numFaults = 0;          // tracks the number of page faults
int size = 0;               // stores the number of requests
int fsize = 0;              // the number of frames we have

FILE* file;

struct frame {
    int pID;			    // a page's unique ID
    int fID;                // the frame where a page is being stored
    int operation;          /* Tracks the kind of operation made on the frame
                            0: No Operation
                            1: Loaded
                            2: Replaced
                            3: Accessed
                            */
    int last_used;          // the last time the page was requested
    int next_used;	        // the next time the page will be requested
    int is_empty;           // bool for whether or not the frame is empty
};


/*======================================================
          AUXILIARY FUNCTION DECLARATION BLOCK
======================================================*/

// The below functions all contain setup necessary to run this
// program
int isValidFile(const char* fname);
int* getInput(const char* fname, int* f, int* size);
void initFrames(struct frame frames[]);
// The below functions are part of a radix sort used by
// all three schedulers
void sortFrameID(struct frame frames[]);
// The below function is part of a radix sort used by
// FIFO and LRU
void sortLastUsed(struct frame frames[]);
// The below function is part of a radix sort uniquely used
// by OPT
void sortNextUsed(struct frame frames[]);
void sortFrameIDReverse(struct frame frames[]);
// The below functions are used for output
void printFrames(struct frame frames[], int idx);
void printUpdate(struct frame f, struct frame old);


/*======================================================
                   MAIN FUNCTION BLOCK
======================================================*/

int main(int argc, char* argv[]) {

    //local variable declaration
    char reportType[16];
    char fname[50];
    char schedType[5];
    struct frame lastFrame = { 0, 0, 0, 0, 0, 0 };  // stores the frame being replaced, if any

    /* Determine report type */
    if (argc == 3) {
        strcpy(fname, argv[1]);
        strcpy(schedType, argv[2]);
        if (!isValidFile(fname)) {
            fprintf(stderr, "Provided file does not exist\n");
            return(1);
        }
        if (strcmp(schedType, FIFO) == 0)
            strcpy(reportType, FIFO);
        else if (strcmp(schedType, OPT) == 0) {
            strcpy(reportType, OPT);
        }
        else if (strcmp(schedType, LRU) == 0)
            strcpy(reportType, LRU);
        else {
            fprintf(stderr, "usage: scheduler (third) argument must be of format [FIFO|LRU|OPT]\n");
            return(1);
        }
    }
    else {
        fprintf(stderr, "usage: proj4 [input_file] [FIFO|LRU|OPT]\n");
        return(1);
    }

    // This is safe because there is no way for c1 to not have a user defined value
    int * requests = getInput(fname, &fsize, &size);
    struct frame frames[fsize];
    initFrames(frames);

    // Finish initialization
    printf("Scheduling algorithm:  %s\n", reportType);
    printf("Total %d tasks are read from \"%s\". Press 'enter' to start...\n",
        size, fname);
    getchar();

    //printFrames(frames, 0);

    /* Code to perform on demand paging */
    // matched is used as a boolean for whether we found the page
    int j = 0, matched = 0, modified_frame = 0;
    for (int i = 0; i < size; i++) {
        matched = 0;
        sortFrameID(frames);
        // sorted in frame order. This can be used for finding empty slots
        // and matches.
        for (j = 0; j < fsize; j++) {
            if (frames[j].pID == *(requests + i) && frames[j].is_empty == 0) {
                if (strcmp(reportType, LRU) == 0) {
                    frames[j].last_used = i;
                }
                if (strcmp(reportType, OPT) == 0) {
                    frames[j].next_used = size + 1;
                    for (int k = (size - 1); k > i; k--) {
                        if (*(requests + k) == frames[j].pID) {
                            frames[j].next_used = k;
                        }
                    }
                }
                frames[j].operation = 3;
                // we found the page
                matched = 1;
                break;
            }
            else if (frames[j].is_empty) {
                frames[j].pID = *(requests + i);
                frames[j].is_empty = 0;
                if (strcmp(reportType, OPT) == 0) {
                    frames[j].next_used = size + 1;
                    for (int k = (size - 1); k > i; k--) {
                        if (*(requests + k) == frames[j].pID) {
                            frames[j].next_used = k;
                        }
                    }
                }
                else {
                    frames[j].last_used = i;
                }
                frames[j].operation = 1;
                // we found a spot to put the page
                matched = 1;
                numFaults += 1;
                break;
            }
        }
        // sort our frames based on how bad we want them out
        // as determined by the scheduling algorithm
        if (strcmp(reportType, OPT) == 0) {
            // sorting by frame ID in reverse needed because next_used
            // is another reverse sort.
            sortFrameIDReverse(frames);
            sortNextUsed(frames);
        }
        else {
            sortLastUsed(frames);
        }

        /* at this point if matched is 0, the page isn't in frame.
        We're alerady sorted the frames by exit priority, so frames[0]
        should contain the highest priority page to replace
        (notably frames[0] isn't actually always frame 0).
        Go ahead and make that switch, but only if matched == 0 */
        if (matched == 0) {
            lastFrame = frames[0];
            frames[0].pID = *(requests + i);
            // we don't need to change the frameID
            frames[0].operation = 2;
            // we don't need to change the empty status
            if (strcmp(reportType, OPT) == 0) {
                frames[0].next_used = size + 1;
                for (int k = (size - 1); k > i; k--) {
                    if (*(requests + k) == frames[0].pID) {
                        frames[0].next_used = k;
                    }
                }
            }
            else {
                frames[0].last_used = i;
            }
            numFaults += 1;
        }
        for (j = 0; j < fsize; j++) {
            if (frames[j].pID == *(requests + i) && frames[j].is_empty == 0)
                break;
        }
        //printFrames(frames, i);
        printUpdate(frames[j], lastFrame);
    }

    printf("%d page faults\n", numFaults);

    free(requests);

    return 0;
}


/*========================================================
           AUXILIARY FUNCTION DEFINITION BLOCK
========================================================*/

/*--------------------------------------------------------
                  SETUP FUNCTIONS
--------------------------------------------------------*/

// A function to verify that the user provided file is real and can be read
int isValidFile(const char* fname) {
    if (access(fname, R_OK) != -1) {
        return 1;
    }
    else {
        return 0;
    }
}

// A function to get the input from the provided file
int * getInput(const char* fname, int* f, int* size) {
    char line[LB_SIZE];
    // p is a garbage variable to get the highest idx of a page
    // we really don't care about it
    int i = 0, p = 0;
    file = fopen(fname, "r");
    //read file
    fgets(line, LB_SIZE + 1, file);
    sscanf(line, "%d %d %d", &p, f, size);
    int *requests = (int*) malloc(sizeof(int) * (size_t)(*size));
    while (fgets(line, LB_SIZE + 1, file) != NULL) {
        sscanf(line, "%d", &p);
        *(requests + i) = p;
        i++;
    }
    fclose(file);
    return requests;
}

// A function to initialize our array of frames
void initFrames(struct frame frames[]) {
    for (int i = 0; i < fsize; i++) {
        frames[i] = (struct frame) {.pID = 0, .fID = i, .operation = 0 , .last_used = 0, 
                            .next_used = size + 1, .is_empty = 1 };
    }
}

/*--------------------------------------------------------
                    UNIVERSAL SORTING
--------------------------------------------------------*/

// A function to sort our input based on PID and Arrival time
void sortFrameID(struct frame frames[]) {
    struct frame aux[fsize];
    int largest = 0, i = 0;

    for (i = 0; i < fsize; i++) {
        aux[i] = frames[i];
        // Get the largest arrival time so we can count sort 
        if (frames[i].fID > largest) {
            largest = frames[i].fID;
        }
    }
    // Create a count array to store count of individual
    // frames by their fID's
    int count[largest + 1];

    for (i = 0; i <= largest; i++) {
        count[i] = 0;
    }
    for (i = 0; i < fsize; i++) {
        count[frames[i].fID]++;
    }
    // Change count[i] so that count[i] now contains actual 
    // position of this page in output array 
    for (i = 1; i <= largest; i++)
        count[i] += count[i - 1];
    // Build the aux output array 
    for (i = fsize - 1; i >= 0; i--) {
        aux[count[frames[i].fID] - 1] = frames[i];
        count[frames[i].fID]--;
    }
    // Copy aux to frames, so that frames now 
    // contains frames sorted by fID
    for (i = 0; i < fsize; i++)
        frames[i] = aux[i];
}

/*--------------------------------------------------------
                    LRU SORTING
--------------------------------------------------------*/

// Count sort our frames by their last_used time
void sortLastUsed(struct frame frames[]) {
    struct frame aux[fsize];
    int largest = 0, i = 0;

    for (i = 0; i < fsize; i++) {
        aux[i] = frames[i];
        // Get the largest arrival time so we can count sort 
        if (frames[i].last_used > largest) {
            largest = frames[i].last_used;
        }
    }
    // Create a count array to store count of individual
    // frames by their fID's
    int count[largest + 1];

    for (i = 0; i <= largest; i++) {
        count[i] = 0;
    }
    for (i = 0; i < fsize; i++) {
        count[frames[i].last_used]++;
    }
    // Change count[i] so that count[i] now contains actual 
    // position of this page in output array 
    for (i = 1; i <= largest; i++)
        count[i] += count[i - 1];
    // Build the aux output array 
    for (i = fsize - 1; i >= 0; i--) {
        aux[count[frames[i].last_used] - 1] = frames[i];
        count[frames[i].last_used]--;
    }
    // Copy aux to frames, so that frames now 
    // contains frames sorted by last_used
    for (i = 0; i < fsize; i++)
        frames[i] = aux[i];
}

/*--------------------------------------------------------
                    OPT SORTING
--------------------------------------------------------*/

// count sort our frames by their next_used time
void sortNextUsed(struct frame frames[]) {
    struct frame aux[fsize];
    int largest = 0, i = 0;

    for (i = 0; i < fsize; i++) {
        aux[i] = frames[i];
        // Get the largest arrival time so we can count sort 
        if (frames[i].next_used > largest) {
            largest = frames[i].next_used;
        }
    }
    // Create a count array to store count of individual
    // frames by their fID's
    int count[largest + 1];

    for (i = 0; i <= largest; i++) {
        count[i] = 0;
    }
    for (i = 0; i < fsize; i++) {
        count[frames[i].next_used]++;
    }
    // Change count[i] so that count[i] now contains actual 
    // position of this page in output array 
    for (i = 1; i <= largest; i++)
        count[i] += count[i - 1];
    // Build the aux output array 
    for (i = fsize - 1; i >= 0; i--) {
        aux[count[frames[i].next_used] - 1] = frames[i];
        count[frames[i].next_used]--;
    }
    // Copy aux to frames, so that frames now 
    // contains frames sorted by next_used
    for (i = 0; i < fsize; i++)
        frames[(fsize - 1) - i] = aux[i];
}

// A function to sort our input based on PID and Arrival time
void sortFrameIDReverse(struct frame frames[]) {
    struct frame aux[fsize];
    int largest = 0, i = 0;

    for (i = 0; i < fsize; i++) {
        aux[i] = frames[i];
        // Get the largest arrival time so we can count sort 
        if (frames[i].fID > largest) {
            largest = frames[i].fID;
        }
    }
    // Create a count array to store count of individual
    // frames by their fID's
    int count[largest + 1];

    for (i = 0; i <= largest; i++) {
        count[i] = 0;
    }
    for (i = 0; i < fsize; i++) {
        count[frames[i].fID]++;
    }
    // Change count[i] so that count[i] now contains actual 
    // position of this page in output array 
    for (i = 1; i <= largest; i++)
        count[i] += count[i - 1];
    // Build the aux output array 
    for (i = fsize - 1; i >= 0; i--) {
        aux[count[frames[i].fID] - 1] = frames[i];
        count[frames[i].fID]--;
    }
    // Copy aux to frames, so that frames now 
    // contains frames sorted by fID
    for (i = 0; i < fsize; i++)
        frames[(fsize - 1) - i] = aux[i];
}

/*--------------------------------------------------------
                   WE PRINT STUFF HERE
--------------------------------------------------------*/

//test function to print the frames array. Used strictly for debugging
void printFrames(struct frame frames[], int idx) {
    for (int i = 0; i < fsize; i++) {
        printf("%d %d %d %d %d %d\n", frames[i].pID, frames[i].fID,
            frames[i].operation, frames[i].last_used, frames[i].next_used,
            frames[i].is_empty);
    }
    printf("IDX %d\n", idx);
}

// Function to handle basic printing of time update values
void printUpdate(struct frame f, struct frame old) {
    if((f.operation == 1) || (f.operation == 2)){
        if (f.operation == 2) {
            printf("Page %d unloaded from Frame %d, ", old.pID, old.fID);
        }
        printf("Page %d loaded into Frame %d\n", f.pID, f.fID);
    }
    else if (f.operation == 3) {
        printf("Page %d already in Frame %d\n", f.pID, f.fID);
    }
}
