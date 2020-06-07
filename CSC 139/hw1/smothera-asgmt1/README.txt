To run the associated program:
1) execute "g++ -o oberserver smothera-asgmt1.cpp -std=gnu++0x" from the directory 
   where this readme is located.
2) To run in standard mode, execute observer (EG: "./observer")
3) To run in short mode, execute observer -s (EG: "./observer -s")
4) To run in long mode:
   execute observer -l interval duration
   WHERE: 
   -interval is the amount of time (in seconds) 
    between polling for CPU load info
   -duration is the amount of time (in seconds)
    over which to poll for CPU load info
   -EG: ./observer -l 10 60
