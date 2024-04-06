#ifndef schedulerH
#define schedulerH

#include <queue>
#include "process.h"

using namespace std;

class Scheduler {
 public:
    Scheduler(vector<Process*> process_list);
    ~Scheduler();
    
    int schedule(vector<Process*> process_list);
    
 private:
    queue<int> process_queue;
};

#endif

