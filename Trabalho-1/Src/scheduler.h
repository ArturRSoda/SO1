#ifndef schedulerH
#define schedulerH

#include <vector>
#include <algorithm>
#include <iostream>
#include "process.h"

using namespace std;

class Scheduler {
 public:
    Scheduler(vector<Process*> process_list);
    void defineAlgorithm(int a);
    int getActiveProcess();
    
    void schedule(vector<Process*> process_list_);
    
 private:

    void sort(bool (Scheduler::*func)(int,int));
    bool compareByStartDate(int a, int b);
    bool compareByPriority(int a, int b);

    void FCFS();
    void staticPrioritySchedule();

    vector<int> processID_vector;
    vector<Process*> process_list;
    int algorithm;
};

#endif

