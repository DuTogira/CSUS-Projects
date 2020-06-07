To run the associated program:
1) execute "gcc -o proj2 smothera-asgmt2.c -std=c99" from the directory
   where this readme is located.
2) Execute proj2 with the desired input file and scheduler name
   based on the format: 
   "./proj2 [input.file] [FCFS|RR|SRTF]"
   EG: "./proj2 input.1 SRTF"
3) To run the Round Robin (RR) scheduler, you must also specify an interval
   based on the format:
   "./proj2 [input.file] RR [interval]"
   EG: "./proj2 input.1 RR 2"
