#ifndef processorH
#define processorH

#include <time.h>
#include "fileReader.h"
#include "process.h"
#include "scheduler.h"

class Processor {
 public:
    Processor(string file_name);
    ~Processor();

    void run();

 private:
    void loadProcess(vector<parameters*> pams);
    void changeContext();
    void checkStartDate();
    void checkTerminated();
    void printStatus();
    void updateActiveProcess(int active_process);
    void chooseAlgorithm();

    int id_count = 0;
    Scheduler* scheduler;
    vector<Process*> process_list;
    int old_process;
    int active_process;
    vector<int> context;
    vector<vector<int>> context_table;
    int context_change_counter = 0;
    time_t time_val = time(0);
    int time_counter = 0;
};

#endif
