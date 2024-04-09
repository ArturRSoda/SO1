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
    void printContext();
    void changeContext(int old_process, int active_process);
    void checkStartDate();
    void checkTerminated();
    void printStatus();
    void updateActiveProcess(int active_process);
    void chooseAlgorithm();
    void printTimes();


    int id_count = 0;
    int preemption_counter = 0;
    Scheduler* scheduler;
    vector<Process*> process_list;
    int old_process;
    int active_process;

    uint64_t SP;
    uint64_t PC;
    uint64_t ST;
    vector<uint64_t> REG;

    int context_change_counter = 0;
    time_t time_val = time(0);
    int time_counter = 0;
};

#endif
