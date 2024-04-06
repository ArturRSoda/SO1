#ifndef processorH
#define processorH

#include <chrono>
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
    void printStatus();

    int id_count = 0;
    Scheduler* scheduler;
    vector<Process*> process_list;
    int old_process;
    int active_process;
    vector<int> context;
    vector<vector<int>> context_table;
    int context_change_counter = 0;
    chrono::time_point<chrono::system_clock> time;
};

#endif
