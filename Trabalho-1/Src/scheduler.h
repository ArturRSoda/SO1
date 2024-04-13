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

    void sortVector(bool (Scheduler::*func)(int,int));
    void sort(bool (Scheduler::*func)(int,int));
    bool compareByAbsDeadline(int a, int b);
    bool compareByPriority(int a, int b);

    vector<int> processID_vector;
    vector<Process*> process_list;
    int algorithm;
};

#endif

